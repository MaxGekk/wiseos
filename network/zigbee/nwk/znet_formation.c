//  vim:encoding=utf-8
/*! @file  znet_formation.c
 *  @brief Формирование сети координатором
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1  
 *
 *      Модуль отвечает за формирование новой сети или переконфигурирование существующей.
 *  Предоставляется реализация примитивов NLME-NETWORK-FORMATION.request ( раздел 3.3.3.1 )
 *  и NLME-NETWORK-FORMATION.confirm ( раздел 3.3.3.2 ).         
 *
 *  */

#include    <math.h>
#include    <ztypes.h>
#include    <zcall.h>
#include    <zigconf.h>
#include    <zneighbor.h>
#include    <zmac_const.h>
#include    <zmac_attr.h>
#include    <zmac_beacon.h>
#include    <zmac_scan.h>
#include    <zmac_start.h>
#include    <znet_attr.h>
#include    <znet_beacon.h>
#include    <znet_formation.h>

ZCALL_USES( ZCALL_MAC_SCAN_ED );
ZCALL_USES( ZCALL_MAC_SCAN_ACTIVE );
ZCALL_USES( ZCALL_MAC_START );

#define     TOTAL_CHANNELS  32
#define     INVALID_NODE_COUNT  255
#define     INVALID_CHANNEL     255

/* Формирование сети завершено */
void    znet_form_done( zcall_t     *mc )
{
    zcall_t     *zc = mc->assoc_call;
    zargs_nform_t   *narg = (zargs_nform_t *)(zc->args);
    zargs_mstart_t  *arg = (zargs_mstart_t *)(mc->args);

    narg->status = arg->status;
    if( narg->status == SUCCESS ) {
        /* Сеть успешно функционирует. Устанавливаем внутренние атрибуты */
        nwkExtraAttr.type = ZIGBEE_COORD;
        nwkExtraAttr.is_joined = 0;
        nwkExtraAttr.channel = arg->channel;
        nwkExtraAttr.depth = 0;
    } 
    zcall_del( mc );
    zcall_return( zc );
    return;
}

/* Поиск неиспользуемого короткого panid на канале. 
 * В случае неудачи возвращается 0xFFFF */
static s_panid_t   znet_unused_spanid(uint8_t   channel)
{
    neighbor_t  *nbr_ptr;
    s_panid_t   panid;
    uint16_t    attempt;

    /* Алгоритм нахождения псевдослучайного короткого идентификатора сети (panid) следующий:
     *  1. Псевдослучайным образом выбираем panid и проверяем по таблице соседей, есть ли узлы
     *  с таким panid на заданном канале. 
     *  2. Если такой узел обнаружен, то повторяем  первый шаг. Итак для выбора panid эту процедуру
     *  нужно повторить максимум число раз, равное размеру таблицы соседей + 1.  */
    panid = random16() % 0x3FFF, attempt = 0, nbr_ptr = nbr_next(0);
    while( IN_TABLE(nbr_ptr) ) {
        if( (nbr_ptr->channel == channel)&&(nbr_ptr->s_panid == panid ) ) {
            if( attempt++ < (Z_NEIGHBOR_TABLE_SIZE+1) ) {
                panid = (panid + 1) % 0x3FFF;
                nbr_ptr = nbr_next(0);
                continue;
            }
            return 0xFFFF;
        }
        nbr_ptr = nbr_next( nbr_ptr );
    }
    return panid;
}

/* Окончено активное сканирование каналов */
void    znet_form_start( zcall_t    *mc )
{
    zcall_t     *zc = mc->assoc_call;
    zargs_nform_t   *narg = (zargs_nform_t *)(zc->args);
    zargs_ascan_t   *arg = (zargs_ascan_t *)(mc->args);

    if( arg->status == SUCCESS ) {
        /* Активное сканирование успешно проведено */
        uint32_t  channels = arg->channels & narg->channels;  /* Битовая маска просканированных каналов */
        if( channels ) {    /* Есть каналы, который удалось просканировать */
            uint16_t    i;
            neighbor_t    *nbr_ptr;
            /* Определяем канал с наименьшим числом узлов */
            uint8_t  node_count[ TOTAL_CHANNELS ];  /* Массив числа узлов на каждом канале */
            uint8_t  opt_channel = INVALID_CHANNEL;
            uint8_t  min_node = INVALID_NODE_COUNT;
            /* Инициализация счётчиков узлов на каналах */
            for( i=0; i<TOTAL_CHANNELS; i++, channels >>= 1 )
                node_count[i] = (channels & 1)?0:INVALID_NODE_COUNT;
            /* Подсчёт числа узлов на каждом канале */
            nbr_ptr = nbr_next(0);
            while( IN_TABLE(nbr_ptr) ) {
                if( node_count[nbr_ptr->channel]!=INVALID_NODE_COUNT )
                    node_count[ nbr_ptr->channel ] += 1;
                nbr_ptr = nbr_next( nbr_ptr );
            }
            /* Поиск канала с наименьшим числом узлов */
            for( i=0; i<TOTAL_CHANNELS; i++ )            
                if( (node_count[i] != INVALID_NODE_COUNT)&&(node_count[i]<min_node) ) {
                    min_node = node_count[i];
                    opt_channel = i;
                }
            if( opt_channel != INVALID_CHANNEL ) {
                /* Найден канал с наименьшим числом узлов */
                zargs_mstart_t  *start_arg = (zargs_mstart_t *)(mc->args);
                /* Ищем не используемый на выбранном канале короткий идентификатор сети */
                start_arg->panid = znet_unused_spanid( opt_channel );
                if( start_arg->panid < 0xFFFF ) {
                    /* Найден неиспользуемый идентификатор сети. */
                    { /* Обновляем beacon payload */
                      uint8_t bcn_payload[Z_BCN_PAYLOAD_SIZE];
                      SET_PROTOCOL_ID(bcn_payload, 0);
                      SET_STACK_PROFILE(bcn_payload, nwkStackProfile );
                      SET_PROTOCOL_VERSION(bcn_payload, nwkProtocolVersion );
                      SET_ROUT_CAPACITY(bcn_payload, 1 );
                      SET_DEPTH(bcn_payload, 0 );
                      SET_END_CAPACITY(bcn_payload, 1 );
                      SET_EPANID( bcn_payload, &nwkExtendedPANID);
                      { uint32_t _txoffset=0; SET_TX_OFFSET( bcn_payload, &_txoffset); }
                      bcn_payload_set( bcn_payload );
                    }
                    /* Готовим аргументы для вызова MLME_START.request */
                    start_arg->channel = opt_channel;
                    start_arg->pan_coord = 1;
                    start_arg->batt_life_ext = narg->batt_life_ext;
                    start_arg->realignment = 0;
                    start_arg->beacon_order = narg->beacon_order;
                    start_arg->sf_order = narg->sf_order;
                    macPANID = start_arg->panid;
                    macShortAddress = 0x0000;
                    if( nwkExtendedPANID == 0 ) nwkExtendedPANID = aExtendedAddress;
                    ZCALL_INIT( mc, ZCALL_MAC_START, znet_form_done );
                    if( IS_OK(zcall_invoke(mc)) )
                        return;
                }
            }
        }
        narg->status = NWK_STARTUP_FAILURE;
    } else narg->status = arg->status;
    zcall_del( mc );
    zcall_return( zc );
    return;
}

/* Окончание детектирования уровня энергии на каналах */
void    znet_form_ascan( zcall_t   *mc )
{
    zcall_t     *zc = mc->assoc_call;
    zargs_nform_t   *narg = (zargs_nform_t *)(zc->args);
    zargs_escan_t   *arg = (zargs_escan_t *)(mc->args);

    if( arg->status == SUCCESS) {
        /* Уровень энергии на каналах успешно измерян */
        uint32_t  ascan_channels = arg->channels & narg->channels;
        if( ascan_channels ) { /* Есть каналы с низким уровнем энергии */
            zargs_ascan_t   *arg = (zargs_ascan_t *)(mc->args);
            /* Инициируем процедуру активного сканирования */
            ZCALL_INIT( mc, ZCALL_MAC_SCAN_ACTIVE, znet_form_start );
            arg->channels = ascan_channels;
            arg->duration = narg->duration;
            nbr_clean(); /* XXX удаляем все записи из таблицы соседей */
            macAutoRequest = FALSE; /* Будем обрабатывать каждый кадр-маяк в отдельности 
                                     * через примитив MLME-BEACON-NOTIFY */
            if( IS_OK(zcall_invoke(mc)) )
                return;
        }
        narg->status = NWK_STARTUP_FAILURE;
    } else narg->status = arg->status;
    zcall_del( mc );
    zcall_return( zc );
    return;
}

/* Начало процедуры формирования сети */
void    znet_form_escan( zcall_t     *zc )
{
    zcall_t     *mc;
    zargs_nform_t   *arg = (zargs_nform_t   *)(zc->args);

    /* Создаём вызов для ed-scan. Далее этот вызов будет использоваться для других целей
     * ( для активного сканирования, например ) */
    mc = zcall_new();
    if( (zcall_t *)0 == mc )
        goto err_nform_ret; /* Не удалось создать канальный вызов */
    mc->assoc_call = zc;    /* Связываем вновь созданный канальный вызов с сетевым */

    if( nwkExtraAttr.type == ZIGBEE_COORD ) {
        /* Сеть уже создана, необходимо поменять параметры */
        zargs_mstart_t  *marg = (zargs_mstart_t *)(mc->args);
        /* Инициализируем канальный вызов для переконфигурирования сети */
        ZCALL_INIT( mc, ZCALL_MAC_START, znet_form_done );
        marg->panid = macPANID;
        marg->channel = nwkExtraAttr.channel;
        marg->pan_coord = 1;    /* Флаг того, что нужно послать команду переконфигурирования дочерним узлам */
        marg->batt_life_ext = arg->batt_life_ext;
        marg->realignment = 1;
        marg->beacon_order = arg->beacon_order;
        marg->sf_order = arg->sf_order;
    } else {
        /* Создаётся новая сеть */
        zargs_escan_t   *marg;
        /* Процедуру формирования новой сети начинаем с детектирования энергии на каналах,
         * чтобы выбрать наименее занятый канал. */
        ZCALL_INIT( mc, ZCALL_MAC_SCAN_ED, znet_form_ascan );
        marg = (zargs_escan_t *)(mc->args);
        marg->channels = arg->channels; /* Битовая маска каналов, которые нужно просканировать */
        marg->duration = arg->duration; /* Параметр, определяющий продолжительность сканирования */
        marg->max_energy = Z_MAX_CHANNEL_ENERGY;    /* Максимально допустимый уровень энергии на канале */
    }
    if( IS_OK( zcall_invoke(mc) ) )     /* Выполняем канальный вызов */ 
        return;
    /* Не удалось произвести канальный вызов, подчищаем хвосты */
    zcall_del(mc);
err_nform_ret: /* Произошла ошибка, извещаем об этом вышележащий уровень */
    arg->status = NWK_INVALID_REQUEST;
    zcall_return(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_NET_FORMATION, znet_form_escan );


