//  vim:encoding=utf-8
/*! @file  znet_discovery.c
 *  @brief Реализация обнаружения соседних сетей
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *
 *      Модуль осуществляет поиск соседних сетей. Реализуются
 *  интерфейсы NLME-NETWORK-DISCOVERY.request и NLME-NETWORK-DISCOVERY.confirm.
 *  Более подробную информацию по работе модуля можно найти в разделах 3.3.2.1
 *  и 3.3.2.2.
 *
 *  */

#include    <ztypes.h>
#include    <zcall.h>
#include    <zneighbor.h>
#include    <zmac_scan.h>
#include    <znet_discovery.h>

ZCALL_USES( ZCALL_MAC_SCAN_PASSIVE );
ZCALL_USES( ZCALL_MAC_SCAN_ACTIVE );

/* Сканирование закончено. Обработка результатов. */
void    znet_discovery_done( zcall_t  *mc )
{
    zcall_t *zc = mc->assoc_call;
    zargs_ndisc_t   *narg = (zargs_ndisc_t *)(zc->args);
    ASSERT( (zcall_t *)0 == zc, "Error: zc is null pointer");

    narg->status = ZARG( mc, zargs_ascan_t, status );
    if( narg->status == SUCCESS ) {
        neighbor_t  *nbr;
        /* Сканирование успешно проведено */
        /* Считаем все узла в таблице соседей потенциальными родителями */
        for( nbr=nbr_next(0); IN_TABLE(nbr); nbr = nbr_next(nbr) )
            nbr->potential_parent = 1;
    }
    zcall_del( mc );
    zcall_return( zc );
    return;
}

/* Обработка запроса на поиск сетей */
void    znet_discovery( zcall_t *zc )
{
    zcall_t     *mc;    /* Указатель на вызов канального уровня для осуществления
                         * процедуры пассивного или активного сканирования */
    zargs_ndisc_t   *narg = (zargs_ndisc_t *)(zc->args);    /* Указатель на аргументы сетевого вызова */
    zargs_ascan_t   *marg;
    /* Создаём новый вызов для сканирования каналов */
    mc = zcall_new();
    marg = (zargs_ascan_t *)(mc->args);    /* Указатель на аргументы канального вызова */

    if( (zcall_t *)0 != mc ) {
        mc->assoc_call = zc;
        /* Вызов успешно создан */
        marg->channels = narg->channels;
        marg->duration = narg->duration;
        /* Сначала пробуем вызвать активное сканирование */
        ZCALL_INIT( zc, ZCALL_MAC_SCAN_ACTIVE, znet_discovery_done ); 
        if( IS_OK(zcall_invoke(zc)) )
            return; /* Инициировано активное сканирование. Выходим и дожидаемся окончания */
        /* Ативное сканирование не поддерживается. Пробуем провести пассивное сканирование.*/
        ZCALL_INIT( zc, ZCALL_MAC_SCAN_PASSIVE, znet_discovery_done ); 
        if( IS_OK(zcall_invoke(zc)) )
            return; /* Выходим и дожидаемся окончания сканирования */
    }
    narg->status = NWK_INVALID_REQUEST;
    zcall_return(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_NET_DISCOVERY, znet_discovery );

/* Функция-итератор по сетям */
network_t*   net_next( network_t  *nwk_ptr, uint16_t *perm_join )
{
    neighbor_t  *nbr_ptr;   
    /* Находим узел - потенциальный представитель следующей сети */
    nwk_ptr = nbr_next( nwk_ptr );
    while( IN_TABLE( nwk_ptr ) ) {
        /* Проверяем все записи до nwk_ptr. Не содержат ли они тот же ExtendedPANID */
        nbr_ptr = nbr_next(0);
        while( nbr_ptr->e_panid != nwk_ptr->e_panid )
            nbr_ptr = nbr_next( nbr_ptr );
        if( nwk_ptr == nbr_ptr  )  { /* Найдена новая сеть.  */
            if( perm_join == 0 ) break; /* Пользователь не хочет знать можно ли подключаться к 
                * сети или нет */
            *perm_join = nwk_ptr->permit_joining;
            if( *perm_join )  break;    /* Уже первый представитель сети разрешает подключение */
            /* Проверяем все узлы этой сети далее по таблице соседей, разрешают ли они подклбчение */
            nbr_ptr = nbr_next( nwk_ptr );
            while( IN_TABLE(nbr_ptr) ) {
                if( (nbr_ptr->e_panid == nwk_ptr->e_panid)&&(*perm_join = nbr_ptr->permit_joining)) 
                    break;  /* Найден узел сети, разрешающий присоединение к себе. Дальше можно не искать. */
                nbr_ptr = nbr_next(nbr_ptr);
            }
            /* Просмотрены все узлы новой сети. В *perm_join записан 1, если сеть разрешает присоединение
             * иначе в *perm_join записан 0. */
            break; /* Выходим, так как новая сеть найдена */
        }
        nwk_ptr = nbr_next( nwk_ptr );
    }
    return nwk_ptr;
}

