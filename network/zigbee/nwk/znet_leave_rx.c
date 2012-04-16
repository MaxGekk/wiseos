//  vim:encoding=utf-8
/*! @file  znet_leave_rx.c
 *  @brief Обработка принятых команд о выходе из сети
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    2
 *
 *      Обработка принятых команд на выход из сети или извещений
 *  о выходе из сети других узлов. Более подробную информацию можно
 *  найти в разделах 3.3.8.2 и 3.7.1.8.3.           
 *
 *  */

#include    <debug.h>
#include    <ztypes.h>
#include    <zcall.h>
#include    <zneighbor.h>
#include    <zpacket.h>
#include    <zmac_const.h>
#include    <zmac_rx.h>
#include    <zmac_tx.h>
#include    <znet_attr.h>
#include    <znet_relative.h>
#include    <znet_leave.h>
#include    <znet_field.h>

ZCALL_USES( ZCALL_MAC_TX_BROADCAST );
ZCALL_USES( ZCALL_LEAVE_INDICATION );
ZCALL_USES( ZCALL_LEAVE_SELF_INDICATION );

/* Макрос возвращает байт с опциями команды leave */
#define  LEAVE_OPT(payload) (*(((uint8_t *)payload)+1))

/* Поиск в таблице соседей узла с заданным длинным адресом */
static neighbor_t*     nbr_by_eaddr( e_addr_t  *addr )
{
    neighbor_t  *nbr_ptr = nbr_next(0);
    while( IN_TABLE( nbr_ptr ) ) {
        if( nbr_ptr->ext_addr == *addr )
            break;
        nbr_ptr = nbr_next( nbr_ptr );
    }
    return nbr_ptr;
}

/* Извещение вышележащего уровня о выходе из сети узла.
 * Данная функция вызывается на роутере после отправки широковещательного
 * пакета дочерним узлам о нашем выходе из сети */
static void    leave_notify( zcall_t     *mac_tx )
{
    zcall_t *zc = mac_tx->assoc_call;
    zpkt_free( ZARG( mac_tx, zargs_mac_btx_t, pkt ) );
    zcall_del( mac_tx );
    if( IS_OK( zcall_invoke(zc) ) )
        return;
    zcall_del(zc);
    return;
}

/* Формирование широковещательного пакета-команды на выход из сети.
 * Данный пакет будет отправлен ближайшим родственным узлам. */
static result_t    leave_pkt( field_t  *field_ptr, uint8_t cmd_opt )
{
    void        *fptr;
    /* Пакет является командой */
    SET_FRAME_CTRL( field_ptr, FC_CMD | FC_PROTOCOL_VER | FC_SADDR);
    SET_DST_ADDR( field_ptr, 0xFFFF );      /* Всем узлам */ 
    SET_SRC_ADDR( field_ptr, nwkShortAddress );
    SET_RADIUS( field_ptr, 1 );     /* Непосредственно связанным с данным узлом */
    SET_SEQ_NUM( field_ptr, nwkSequenceNumber ), ++nwkSequenceNumber;
    SET_SRC_EADDR( field_ptr, aExtendedAddress );
    if((fptr = GET_PAYLOAD( field_ptr )) == 0) return EINVAL;
    *((uint8_t *)fptr) = CMD_LEAVE, fptr = (uint8_t *)fptr + 1;
    *((uint8_t *)fptr) = cmd_opt;
    return ENOERR;
}

/* Обработка команды выхода из сети */
void    znet_leave_cmd( zcall_t  *zc )
{
    field_t     field;
    void        *fptr;
    uint8_t     cmd_opt;
    packet_t    *lc = ZARG(zc,zargs_mdata_rx_t, pkt);

    /* Подготовка к вычитыванию полей */
    if( (fptr = zpkt_body(lc) ) == 0 ) goto leave_cmd_err;
    NET_FIELD_INIT( &field, fptr );
    /* Вычитывание из пакета опций команды */
    if((fptr = GET_PAYLOAD( &field )) == 0) goto leave_cmd_err;
    cmd_opt = LEAVE_OPT(fptr);

    /* Первым делом определяем - это информационный пакет о выходе узла из сети 
     * или команда нам на выход из сети. */
    if( ( cmd_opt & LEAVE_REQUEST ) == LEAVE_REQUEST ) {
        /* Команда на принудительный выход из сети */
        neighbor_t  *parent_ptr = get_parent();
        if( (neighbor_t *)0 == parent_ptr ) goto leave_cmd_err;
        /* Только родитель может отдавать команды на выход из сети */
        /* Вычитывание из пакета короткого адреса источника */
        if((fptr = GET_SRC_ADDR( &field )) == 0) goto leave_cmd_err;
        /* Согласно спецификации нужно проверять короткий адрес родителя (см. стр. 350-351) */
        if( parent_ptr->net_addr != *(SRC_ADDR_TYPE *)fptr ) goto leave_cmd_err;
        /* TODO Вышел из сети наш родитель. Каким-то образом нужно известить об этом канальный 
         * уровень, чтобы он больше не поддерживал с ним связь */
        /* Извещаем вышележащий уровень о нашем выходе из сети */
        /* Переиспользуем вызов zc для этой цели */
        ZCALL_INIT( zc, ZCALL_LEAVE_SELF_INDICATION, 0 );
        ZARG(zc, zargs_sileave_t, rejoin ) = ( cmd_opt & LEAVE_REJOIN )?TRUE:FALSE;
        /* Роутер может иметь детей, поэтому он должен оповестить их о своём выходе из сети */
        if( nwkExtraAttr.type == ZIGBEE_ROUTER ) {
            zcall_t *mac_tx = zcall_new();  /* Создаём новый вызов, для широковещательной отправки */
            if( (zcall_t *)0 != mac_tx ) {
                mac_tx->assoc_call = zc;    /* Связываем новый вызов с вызовом оповещения вышележащего уровня,
                    * для того чтобы после отправки пакета дочерним узлам оповестить верхний уровень о нашем выходе
                    * из сети */
                if( IS_OK( leave_pkt( &field, cmd_opt & ~LEAVE_REQUEST ))) {
                    /* Подготовлен широковещательный пакет */
                    ZCALL_INIT( mac_tx, ZCALL_MAC_TX_BROADCAST, leave_notify );
                    ZARG( mac_tx, zargs_mac_btx_t, pkt ) = lc;
                    if( IS_OK( zcall_invoke( mac_tx ) ) )
                        return;  /* Выполняем отправку пакета дочерним узлам */              
                }
                zcall_del(mac_tx);
            }
        } 
        /* Оповещаем верхний уровень о нашем выходе из сети */
        if( IS_OK(zcall_invoke(zc)) ) {
            zpkt_free(lc);
            return;
        }
    } else {
        neighbor_t  *nbr_ptr;
        /* Какой-то узел извещает о выходе из сети */
        /* Вычитывание из пакета длинного адреса этого узла */
        if((fptr = GET_SRC_EADDR( &field )) == 0) goto leave_cmd_err;
        /* Готовим стековый вызов для оповещения верхнего уровня о выходе из сети узла */
        ZCALL_INIT(zc, ZCALL_LEAVE_INDICATION, 0 );
        ZARG(zc, zargs_ileave_t, dev_addr ) = *(SRC_EADDR_TYPE *)fptr;  /* Длинный адрес вышедшего из сети узла */
        /* Поиск записи в таблице соседей с таким длинным адресом. Чтобы освободить запись, занятую этим узлом. */
        nbr_ptr = nbr_by_eaddr( (SRC_EADDR_TYPE *)fptr );
        if( IN_TABLE(nbr_ptr) ) {
            /* Запись узла найдена в таблице соседей */
            nbr_ptr->busy = 0;  /* Освобождаем её */
            if( nbr_ptr->relationship == NWK_PARENT ) {
                /* TODO Вышел из сети наш родитель. Каким-то образом нужно известить об этом канальный 
                 * уровень, чтобы он больше не поддерживал с ним связь */
                if( (nwkExtraAttr.type == ZIGBEE_ROUTER)&&(cmd_opt & LEAVE_RMCHLD) ) {
                    /* Поскольку у нас могут быть дочерние узлы ( мы роутер ) и родительский узел
                     * просит исключить из сети наших детей, то отправляем им команду на выход */
                    zcall_t     *mac_tx = zcall_new();  /* Создаём вызов, для широковещательной отправки */
                    if( mac_tx != (zcall_t *)0) {
                        mac_tx->assoc_call = zc;    /* Связываем его с вызовом оповещения верхнего уровня, 
                            * чтобы после отправки пакета оповестить верхний уровень */
                        if( IS_OK( leave_pkt( &field, cmd_opt ))) {
                            /* Широковещательный пакет подготовлен. Отправляем его */
                            ZCALL_INIT( mac_tx, ZCALL_MAC_TX_BROADCAST, leave_notify );
                            ZARG( mac_tx, zargs_mac_btx_t, pkt ) = lc;
                            if( IS_OK( zcall_invoke(mac_tx) ) )
                                return; /* Пакет отправлен */
                        }
                        zcall_del(mac_tx);
                    }
               }
           }
        }
        /* Извещаем верхний уровень о выходе из сети узла */
        if( IS_OK( zcall_invoke(zc) ) ) {
            zpkt_free(lc);
            return;
        }
    }
leave_cmd_err:
    zpkt_free(lc);
    zcall_del(zc);
    return; 
}

ZCALL_PROVIDE( ZCALL_NET_LEAVE_RX, znet_leave_cmd );


