//  vim:encoding=utf-8
/*! @file  znet_leave_self.c
 *  @brief Самостоятельный выход из сети
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1    
 *
 *      Модуль позволяет самостоятельно выйти из сети и известить об этом 
 *  ближайшие родственные сети. Предоставляется реализация примитивов 
 *  NLME-LEAVE.request ( см. раздел 3.3.8.1 ) и NLME-LEAVE.confirm ( см. раздел 
 *  3.3.8.3 ). Более подробную информацию можно найти в разделе 3.7.1.8.2 спецификации
 *  ZigBee 2006 года.
 *
 *  */

#include    <debug.h>
#include    <ztypes.h>
#include    <zcall.h>
#include    <zpacket.h>
#include    <zmac_const.h>
#include    <zmac_tx.h>
#include    <znet_attr.h>
#include    <znet_relative.h>
#include    <znet_field.h>
#include    <znet_leave.h>

ZCALL_USES( ZCALL_MAC_TX_UNICAST );
ZCALL_USES( ZCALL_MAC_TX_BROADCAST );

#define     LEAVE_CMD_HEADER_SIZE   \
    (FRAME_CTRL_SIZE+DST_ADDR_SIZE+SRC_ADDR_SIZE+RADIUS_SIZE+SEQ_NUM_SIZE+SRC_EADDR_SIZE)

#define     LEAVE_CMD_PAYLOAD_SIZE 2

/* Формирование пакета, извещающего о самостоятельном выходе узла из сети */
inline packet_t*   leave_self_pkt( uint8_t cmd_opt )
{
    packet_t    *lc;
    field_t     lcfield;
    void    *lcptr;

    /* Создание командного фрейма выхода из сети для родителя.*/
    if( (lc = zpkt_new())==0 ) 
        return (packet_t *)0;

    /* Формирование тела команды */
    lcptr = zpkt_append( lc, LEAVE_CMD_PAYLOAD_SIZE );
    ASSERT( 0==lcptr, "Error: can not append leave cmd payload");
    *((uint8_t *)lcptr) = CMD_LEAVE, lcptr = (uint8_t *)lcptr + 1;
    *((uint8_t *)lcptr) = cmd_opt;
    /* Формирование сетевого заголовка */
    lcptr = zpkt_append( lc, LEAVE_CMD_HEADER_SIZE );
    ASSERT( 0==lcptr, "Error: can not append leave cmd header");
    NET_FIELD_INIT( &lcfield, lcptr );
    SET_FRAME_CTRL( &lcfield, FC_CMD | FC_PROTOCOL_VER | FC_SADDR);
    SET_DST_ADDR( &lcfield, 0xFFFC );   
    SET_SRC_ADDR( &lcfield, nwkShortAddress );
    SET_RADIUS( &lcfield, 1 );
    SET_SEQ_NUM( &lcfield, nwkSequenceNumber ), ++nwkSequenceNumber;
    SET_SRC_EADDR( &lcfield, aExtendedAddress );

    return lc;
}

/* Окончание процедуры отправки уведомления о выходе из сети 
 * ближайшим родственникам */
void    net_leave_self_done( zcall_t *mac_tx )
{
    zcall_t     *zc = mac_tx->assoc_call;

    if( ZCALL_NAME(zc) == ZCALL_LEAVE_SELF_FFD)
        ZARG( zc, zargs_sfleave_t, status ) = ZARG(mac_tx, zargs_mac_btx_t, status );
    else 
        ZARG( zc, zargs_srleave_t, status ) = ZARG(mac_tx, zargs_mac_utx_t, status );
    /* Освобождаем использованный стековый вызов  */
    zcall_del( mac_tx );
    /* Возвращаем статус самостоятельного выхода из сети */
    zcall_return( zc );
    return;
}

/* Обработка запроса на самостоятельный выход из сети */
void    net_leave_self( zcall_t  *zc )
{
    zcall_t     *mac_tx;
    packet_t    *lc;
    uint8_t     cmd_opt = 0;

    if( ZCALL_NAME(zc) == ZCALL_LEAVE_SELF_FFD) {
        /* Инструкции для наших детей, если они у нас есть */
        if( ZARG(zc, zargs_sfleave_t,rejoin) )
            cmd_opt |= LEAVE_REJOIN;
        if( ZARG(zc, zargs_sfleave_t,remove_children) )
            cmd_opt |= LEAVE_RMCHLD;
    }

    /* Создаём leave cmd со всеми сброшенными флагами */
    if( (lc = leave_self_pkt(cmd_opt))==0 ) 
        goto no_free_pkt;

    /* Создание стекового вызова для отправки командного пакета */   
    if( (mac_tx = zcall_new())==0 )
        goto cant_tx;

    if( ZCALL_NAME(zc) == ZCALL_LEAVE_SELF_FFD) {
        /* Выход из сети полнофункционального узла - роутера.
         * Отправляем широковещательный пакет о нашем выходе ближайшим родственникам */
        ZCALL_INIT( mac_tx, ZCALL_MAC_TX_BROADCAST, net_leave_self_done );
        ZARG(mac_tx, zargs_mac_btx_t, pkt ) = lc;   
    } else {
        /* Выход из сети устройства с ограниченной функциональностью - конечного узла */
        neighbor_t *parent_ptr;
        /* Отправляем уведомление только нашему родителю, больше некому */
        ZCALL_INIT( mac_tx, ZCALL_MAC_TX_UNICAST, net_leave_self_done );
        ZARG(mac_tx, zargs_mac_utx_t, pkt ) = lc;
        parent_ptr = get_parent();
        if( !IN_TABLE(parent_ptr) ) {
            /* Очень странно, но у нас нет родителя. Ошибка на верхнем уровне */
            ASSERT(1,"Warning: I don't have a parent");
            goto cant_tx;   
        }
        ZARG(mac_tx, zargs_mac_utx_t, dst ) = parent_ptr;
    }

    mac_tx->assoc_call = zc;
    /* Отправляем уведомление о нашем выходе из сети */
    if( IS_OK(zcall_invoke( mac_tx )) )
        return;
    /* Подчищаем хвосты */
    zcall_del( mac_tx );
cant_tx:
    zpkt_free( lc );
no_free_pkt:    
    if( ZCALL_NAME(zc) == ZCALL_LEAVE_SELF_FFD)
        ZARG( zc, zargs_sfleave_t, status ) = NWK_INVALID_REQUEST;
    else 
        ZARG( zc, zargs_srleave_t, status ) = NWK_INVALID_REQUEST;
    zcall_return(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_LEAVE_SELF_RFD, net_leave_self );
ZCALL_PROVIDE( ZCALL_LEAVE_SELF_FFD, net_leave_self );

