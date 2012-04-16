//  vim:encoding=utf-8
/*! @file  znet_assoc_parent.c
 *  @brief Ассоциация на стороне родителя
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1     
 *
 *      Модуль реализует присоединение с помощью канальной ассоциации на стороне 
 *  родительского узла. Предоставляется реализация примитива NLME_JOIN.indication.
 *  Более подробную информацию можно найти в разделах 3.3.6.2 и 3.7.1.3.1.2.
 *
 *  */

#include    <debug.h>
#include    <zigconf.h>
#include    <zcall.h>
#include    <zneighbor.h>
#include    <zmac_attr.h>
#include    <znet_join.h>
#include    <znet_attr.h>
#include    <znet_addr.h>
#include    <znet_beacon.h>
#include    <znet_relative.h>

ZCALL_USES( ZCALL_NLME_JOIN_INDICATION );
ZCALL_USES( ZCALL_MLME_ASSOC_PARENT );

/* Обработка уведомления об отправке ответа новому дочернему узлу */
void    znet_pjoin_done( zcall_t    *zc )
{
    neighbor_t  *child_ptr;
    zargs_passoc_t  *arg = (zargs_passoc_t *)(zc->args);
    if( arg->status == SUCCESS ) {
        /* Ответ успешно отправлен */
        if( nwkMaxChildren <= child_count() ) {
            /* Достигнут порог для числа детей. Обновим информацию в нашем кадре-маяке */
            bcn_all_cap_off();
        }
        /* Уведомляем вышележащий уровень о новом дочернем узле. */
        ZCALL_INIT( zc, ZCALL_NLME_JOIN_INDICATION, 0 );
        if( IS_OK(zcall_invoke(zc)) )
            return;
    }
    /* Произошла какая-то ошибка. Ищем запись узла в таблице соседей и удаляем её.  */
    child_ptr = child_by_eaddr( arg->dev_addr );
    if( IN_TABLE( child_ptr ) )
        child_ptr->busy = 0;
    zcall_del(zc);
    return;
}

/* Обработка уведомления о запросе ассоциации */
void    znet_assoc_indication( zcall_t  *zc )
{
    neighbor_t  *child_ptr;
    zargs_passoc_t  *arg = (zargs_passoc_t *)(zc->args);
    bool_t  alloc_addr;     /* Логический флаг - выделять ли короткий адрес */

    if( nwkMaxChildren <= child_count() )
        goto assoc_denied;  /* Достигнут максимальный порог числа детей. Больше детей заводить нельзя. */

    child_ptr = nbr_place_by_eaddr( &(arg->dev_addr) ); /* Ищем место в таблице соседей */
    if( !IN_TABLE( child_ptr) )
        goto assoc_denied;  /* Не удалось найти место в таблице соседей */

    alloc_addr = TRUE; 
    if( child_ptr->ext_addr == arg->dev_addr ) {
        /* Уже есть запись об узле в таблице соседей */
        if( ( child_ptr->relationship == NWK_PARENT )||( child_ptr->relationship == NWK_SIBLING ) )
            goto assoc_denied;  /* Родителя и братьев не присоединяем */
        if( ( child_ptr->relationship == NWK_CHILD )
            ||( ( child_ptr->relationship == NWK_PREVIOUS_CHILD )&&( child_ptr->net_addr != 0xFFFF ) ) )
            alloc_addr = FALSE; /* Не выделяем короткий адрес детям, у которых он уже есть */
    }

    if( alloc_addr == TRUE ) {
        arg->assoc_addr = znet_addr_alloc( arg->cap_info.dev_type );
        if( child_ptr->net_addr == 0xFFFF ) {
            /* Не удалось выделить короткий адрес */
            bcn_cap_off( arg->cap_info.dev_type );  /* Сбрасываем флаг разрешения присоединения для данного типа устройств */
            child_ptr->busy = 0;    /* Запись не очень важная. Можно и удалить. */
            goto assoc_denied;
        }
    }
    /* Заносим в таблицу соседей информацию о новом дочернем узле */
    child_ptr->rx_on_when_idle = arg->cap_info.rx_on_when_idle;
    child_ptr->potential_parent = 0;
    child_ptr->permit_joining = 0;
    child_ptr->dev_type = ( arg->cap_info.dev_type == FFD )? ZIGBEE_ROUTER : ZIGBEE_ENDDEV;
    child_ptr->relationship = NWK_CHILD;
    child_ptr->channel = nwkExtraAttr.channel;
    child_ptr->beacon_order = macBeaconOrder;
    child_ptr->depth = nwkExtraAttr.depth+1;
    child_ptr->ext_addr = arg->dev_addr;
    child_ptr->net_addr = arg->assoc_addr;
    child_ptr->e_panid = nwkExtendedPANID;
    /* Отправляем положительный ответ узлу */
    ZCALL_INIT( zc, ZCALL_MLME_ASSOC_PARENT, znet_pjoin_done );
    arg->status = SUCCESS;
    if( IS_ERROR(zcall_invoke( zc )) )
        zcall_del( zc );
    return;       
assoc_denied:
    /* Присоединить узел не получилось. Отправляем узлу отрицательный ответ. */
    arg->assoc_addr = 0xFFFF;
    arg->status = MAC_PAN_ACCESS_DENIED;
    ZCALL_INIT( zc, ZCALL_MLME_ASSOC_PARENT, 0 );   /* Подтверждение нам не нужно */
    if( IS_ERROR(zcall_invoke( zc )) )
        zcall_del( zc );
    return;
}

ZCALL_PROVIDE( ZCALL_MLME_ASSOC_INDICATION, znet_assoc_indication );

