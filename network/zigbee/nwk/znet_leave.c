//  vim:encoding=utf-8
/*! @file  znet_leave.c
 *  @brief Принудительный выход из сети
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1 
 *
 *      Модуль позволяет отправить любому дочернему узлу команду на выход из сети.
 *  Продоставляется реализация примитивов NLME-LEAVE.request ( см. раздел 3.3.8.1 )
 *  и NLME-LEAVE.confirm ( см. раздел 3.3.8.3 ).          
 *
 *  */

#include    <debug.h>
#include    <zigconf.h>
#include    <zcall.h>
#include    <zpacket.h>
#include    <zmac_tx.h>
#include    <znet_attr.h>
#include    <znet_relative.h>
#include    <znet_field.h>
#include    <znet_leave.h>
#include    <znet_beacon.h>

ZCALL_USES( ZCALL_MAC_TX_UNICAST );

#define     LEAVE_CMD_HEADER_SIZE   \
    (FRAME_CTRL_SIZE+DST_ADDR_SIZE+SRC_ADDR_SIZE+RADIUS_SIZE+SEQ_NUM_SIZE+SRC_EADDR_SIZE)

#define     LEAVE_CMD_PAYLOAD_SIZE 2

/* Обновление записи исключаемого узла в таблице соседей */
static void    child_update( zcall_t   *zc, neighbor_t  *child_ptr )
{
    zargs_leave_t   *arg = (zargs_leave_t *)(zc->args);

    if( arg->rejoin ) {
        /* Узел должен переподключиться после выхода из сети */
        child_ptr->relationship = NWK_PREVIOUS_CHILD;   /* Теперь узел уже бывший ребёнок */
        if( arg->reuse_addr ) {
            /* Адрес узла может использоваться новыми дочерними узлами */
            bcn_cap_on( child_ptr->dev_type );  /* На всякий случай объявляем всем, что мы может 
                * подключать к себе узлы с тем же типом, что и у исключённого узла */
            child_ptr->net_addr = 0xFFFF;
        }
        /* Ребёнок остался с адресом, хоть и стал бывшим */
    } else {
        bcn_cap_on( child_ptr->dev_type );
        child_ptr->relationship = NWK_STRANGER; /* Теперь исключённый из сети дочерний узел нам чужой */
    }
}

/* Процедура отправки команды на выход из сети закончена */
void    net_leave_done( zcall_t *mac_tx )
{
    zargs_mac_tx_t  *marg = (zargs_mac_tx_t *)(mac_tx->args);
    zcall_t     *zc = mac_tx->assoc_call;

    ZARG(zc, zargs_leave_t, status) = marg->status;
    if( SUCCESS == marg->status ) {
        /* Команда успешно отправлена */
        child_update( zc, marg->dst );
    } 
    /* Освобождаем занятые ресурсы */
    zpkt_free( marg->pkt );
    zcall_del( mac_tx );
    /* Возвращаем результат вышележащему уровню */
    zcall_return( zc );
    return;
}

/* Обработка запроса на исключение дочернего узла из сети */
void    net_leave( zcall_t  *zc )
{
    zcall_t     *mac_tx;
    packet_t    *lc;
    field_t     lcfield;
    void    *lcptr;
    neighbor_t  *child_ptr;
    zargs_leave_t   *arg = (zargs_leave_t *)(zc->args);

    /* Проверяем, является ли исключаемое из сети устройство нашим дочерним узлом */
    child_ptr = child_by_eaddr( arg->dev_addr );
    if( !IN_TABLE(child_ptr) ) {
        /* Мы не имеем права исключать из сети не наших детей */
        arg->status = NWK_UNKNOWN_DEVICE;
        goto leave_exit;
    }

    if( arg->silent ) {
        /* Удаляем дочерний узел, не сказав ему ничего */
        child_update( zc, child_ptr );
        arg->status = SUCCESS;
        goto leave_exit;
    }

    /* Создание пакета, который будет содержать команду на выход из сети */
    if( (lc = zpkt_new())==0 ) { 
        arg->status = NWK_INVALID_REQUEST;
        goto leave_exit;
    }

    /* Создание стекового вызова для отправки команды на выход из сети */   
    if( (mac_tx = zcall_new())==0 )
        goto cant_tx;

    /* Формирование тела команды */
    lcptr = zpkt_append( lc, LEAVE_CMD_PAYLOAD_SIZE );
    ASSERT( 0==lcptr, "Error: can not append leave cmd payload");
    /* Определяем полезную нагрузку пакета */
    *((uint8_t *)lcptr) = CMD_LEAVE, lcptr=(uint8_t *)lcptr+1;  /* Команда выхода из сети */
    *((uint8_t *)lcptr) = LEAVE_REQUEST;    /* Принудительный выход */
    if( arg->remove_children ) *((uint8_t *)lcptr) |= LEAVE_RMCHLD; /* Исключать ли из сети наших внуков */
    if( arg->rejoin ) *((uint8_t *)lcptr) |= LEAVE_REJOIN;  /* Переподключаться ли к сети после выхода */

    /* Формирование сетевого заголовка */
    lcptr = zpkt_append( lc, LEAVE_CMD_HEADER_SIZE );
    ASSERT( 0==lcptr, "Error: can not append leave cmd header");
    NET_FIELD_INIT( &lcfield, lcptr );
    SET_FRAME_CTRL( &lcfield, FC_CMD | FC_PROTOCOL_VER | FC_DADDR );
    SET_DST_ADDR( &lcfield, child_ptr->net_addr );
    SET_SRC_ADDR( &lcfield, nwkShortAddress );
    SET_RADIUS( &lcfield, 1 );
    SET_SEQ_NUM( &lcfield, nwkSequenceNumber ), ++nwkSequenceNumber;
    SET_DST_EADDR( &lcfield, child_ptr->ext_addr ); /* Именно по длинному адресу наш ребёнок будет определять
        * что команда на выход от нас */

    /* Отправка одноадресного пакета */
    ZCALL_INIT( mac_tx, ZCALL_MAC_TX_UNICAST, net_leave_done );
    ZARG( mac_tx, zargs_mac_utx_t, dst ) = child_ptr;
    ZARG( mac_tx, zargs_mac_utx_t, pkt ) = lc;
    mac_tx->assoc_call = zc;
    if( IS_OK(zcall_invoke( mac_tx )) )
        return;
    zcall_del( mac_tx );
cant_tx:
    zpkt_free( lc );
leave_exit:
    zcall_return(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_LEAVE, net_leave );

