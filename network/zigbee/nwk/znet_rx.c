//  vim:encoding=utf-8
/*! @file  znet_rx.c
 *  @brief Обработка полученных фреймов
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    2
 *
 *      Модуль реализует обработку всех принятых от канального уровня пакетов и,
 *  после анализа этих пакетов, перепоручает их дальнейшую обработку другим более
 *  специализированным модулям сетевого уровня. Более детальное описание работы 
 *  модуля можно найти в разделах 3.7.2.2, 3.7.3.3, 3.7.5, 3.7.6.
 *
 *  */

#include    <debug.h>
#include    <ztypes.h>
#include    <zcall.h>
#include    <zpacket.h>
#include    <zfield.h>
#include    <zmac_rx.h>
#include    <zmac_attr.h>
#include    <znet_attr.h>
#include    <znet_field.h>
#include    <znet_leave.h>
#include    <znet_group.h>
#include    <znet_route.h>
#include    <znet_rx.h>

ZCALL_USES( ZCALL_NET_MULTICAST_ROUTE );
ZCALL_USES( ZCALL_NET_BROADCAST_ROUTE );
ZCALL_USES( ZCALL_NET_UNICAST_ROUTE );
ZCALL_USES( ZCALL_NET_DATA_RX );
ZCALL_USES( ZCALL_NET_LEAVE_RX );

/* Пересылка broadcast-пакета другим узлам */
void    znet_broute( zcall_t  *zc )
{
    ZCALL_INIT( zc, ZCALL_NET_BROADCAST_ROUTE, 0 );  
    if( IS_OK(zcall_invoke(zc)) ) {
        /* Найден модуль пересылки широковещательных пакетов. Отдаём ему
         * дальнейшую обработку пакета. Ответственность за удаление стекового вызова
         * и пакета переходит этому модулю. */
        return;
    }
    /* Никто не взялся обработать пакет, поэтому ничего не остаётся как удалить его. 
     * Ведь мы теперь ответственны за удаление стекового вызова и принятого пакета.
     * */
    zpkt_free( ZARG(zc,zargs_mdata_rx_t, pkt) );
    zcall_del( zc );
}


/* Пересылка multicast-пакета другим членам группы */
void    znet_mroute( zcall_t  *zc )
{
    ZCALL_INIT( zc, ZCALL_NET_MULTICAST_ROUTE, 0 );  
    if( IS_OK(zcall_invoke(zc)) ) {
        /* Найден модуль пересылки групповых пакетов. Отдаём ему
         * дальнейшую обработку пакета. Ответственность за удаление стекового вызова
         * и пакета переходит этому модулю. */
        return;
    }
    /* Удаляем пакет и вызов ( так как мы ответственны за их удаление ), поскольку не 
     * найден ни один модуль-обработчик. */
    zpkt_free( ZARG(zc,zargs_mdata_rx_t, pkt) );
    zcall_del( zc );
}

/* Поиск модуля сетевого уровня, которому можно перепоручить дальнейшую обработку пакета */
result_t   zcall_entrust( zcall_t   *zc, zcall_func_t callback,
      field_t   *field_ptr, FRAME_CTRL_TYPE fc  )
{
    if( ( fc & FC_TYPE_MASK) == FC_DATA ) {
        /* Принят пакет с данным. Связываем дальнейшую обработку стекового вызова с 
         * модулем обработки данных. */
        ZCALL_INIT( zc, ZCALL_NET_DATA_RX, callback );
        return ENOERR;
    } else if( ( fc & FC_TYPE_MASK) == FC_CMD ) {
        /* Принят пакет с командой */
        uint8_t *cmdid_ptr;

        /* Вычитывание из полезной нагрузки пакета идентификатора команды */
        if(( cmdid_ptr = GET_PAYLOAD( field_ptr )) == 0) 
            return EINVAL;

        /* Пересвязывание стекового вызова в соответствии с командой, для перепоручения
         * дальней обработки пакета другому модулю */
        switch( *cmdid_ptr ) {
            case CMD_ROUTE_REQUEST:
            case CMD_ROUTE_REPLAY:
            case CMD_ROUTE_ERROR: return ENOSYS;
            case CMD_LEAVE: ZCALL_INIT( zc, ZCALL_NET_LEAVE_RX, callback ); return ENOERR;
            case CMD_ROUTE_RECORD:
            case CMD_REJOIN_REQUEST:
            case CMD_REJOIN_RESPONSE: return ENOSYS;
            default: break;
        }
    }
    return EINVAL;
}

/* Инициализация структуры field и вычитывание полей frame control и destination address*/
static result_t    get_field( zcall_t *zc, field_t *field_ptr, 
        FRAME_CTRL_TYPE *fc_ptr, DST_ADDR_TYPE   *daddr_ptr )
{
    /* Подготовка к вычитыванию полей */
    void        *fptr;
    if( (fptr = zpkt_body( ZARG(zc,zargs_mdata_rx_t, pkt) ) ) == 0 )
        return EINVAL;
    NET_FIELD_INIT( field_ptr, fptr );

    /* Вычитывание из пакета поля frame control */
    if(( fc_ptr = GET_FRAME_CTRL( field_ptr )) == 0)
        return EINVAL;

    /* Вычитывание из пакета поля адреса назначения */
    if(( daddr_ptr = GET_DST_ADDR( field_ptr )) == 0)
        return EINVAL;

    return ENOERR;
}


/* Обработка всех полученных от канального уровня фреймов */
void    znet_mac_data_rx( zcall_t  *zc )
{
    field_t     field;
    FRAME_CTRL_TYPE fc;
    DST_ADDR_TYPE   daddr;

    /* Для первичного анализа принятых пакетов достаточно полей frame control и адреса назначения.
     * Поэтому первым делом вычитываем эти поля  из заголовка сетевого пакета. */
    if( IS_ERROR(get_field( zc, &field, &fc, &daddr )) )
        goto mdata_err;

    if( (fc & FC_MULTICAST) == FC_MULTICAST ) {
        /* Принят пакет, предназначенный группе узлов ( multicast ). */
        if( IS_OK(zgroup_find(daddr)) && IS_OK(zcall_entrust(zc, znet_mroute, &field, fc)) ) {
            /* Если узел входит в группу назначения, то находим модуль на сетевом уровне, которому можно поручить 
             * дальнейшую обработку пакета. С помощью zcall_entrust связываем обработку zc с этим модулем.
             * */
            if( IS_OK(zcall_invoke(zc))) {
                /* Продолжаем обработку принятого пакета в более специализированном модуле */
                return;
            }
        }
        /* Если узел не принадлежит группе назначения или не удалось перепоручить обработку пакета другому модулю,
         * то пробуем смаршрутизировать этот пакет. Ответственность за удаление пакета и вызова ложиться на 
         * функцию znet_mroute.*/
        znet_mroute(zc);
        return;
    }

    if( daddr < 0xFFF8 ) {
        /* Принят обычный одноадресный пакет ( unicast ). */
        if( (daddr == nwkShortAddress) ) {
            /* Пакет предназначен данному узлу. */
            if( IS_OK(zcall_entrust(zc, znet_mroute, &field, fc)) && IS_OK(zcall_invoke(zc)) ) {
                /* Найден модуль, которому можно перепоручить дальнейшую обработку пакета */
                return;
            } else {
                /* Не нейдено ни одного модуля, которому можно перепоручить обработку пакета. 
                 * Удаляем пакет и выходим. */
                goto mdata_err;
            }
        }
        /* Пакет предназначен не нам. Пробуем смаршрутизировать его. */
        ZCALL_INIT( zc, ZCALL_NET_UNICAST_ROUTE, 0 );  
        if( IS_ERROR(zcall_invoke(zc)) ) {
            /* Модуль маршрутизации unicast-пакетов не найден. Удаляем пакет и выходим. */
            goto mdata_err;
        }
        return;
    }

    if( (daddr == 0xFFFF)
        ||( ( daddr == 0xFFFC )&&( (nwkExtraAttr.type == ZIGBEE_COORD)||(nwkExtraAttr.type == ZIGBEE_ROUTER) ) )
        ||( ( daddr == 0xFFFD )&&( macRxOnWhenIdle == TRUE )) ) {
        /* Принят один из широковещательных пакетов. См. таблицу 3.50 спецификации ZigBee 2006.*/
        if( IS_OK(zcall_entrust(zc, znet_broute, &field, fc))&& IS_OK(zcall_invoke(zc)) )  {
            /* Обнаружен модуль, которому можно перепоручить дальнейшую обработку широковещательного пакета. */
            return;
        }
        /* Не найден ни один модуль, который смог бы обработать пакет. Пробуем смаршрутизировать его. 
         * Ответственность за удаление пакета и вызова ложиться на функцию znet_broute. */
        znet_broute(zc);
        return;
    }

mdata_err:
    /* В процессе обработки пакета произошла ошибка. Удаляем стековый вызов,
     * принятый пакет и выходим. */
    zpkt_free( ZARG(zc,zargs_mdata_rx_t, pkt) );
    zcall_del( zc );
    return;
}

ZCALL_PROVIDE( ZCALL_MAC_DATA_RX, znet_mac_data_rx );

