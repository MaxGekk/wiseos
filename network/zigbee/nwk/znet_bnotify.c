//  vim:encoding=utf-8
/*! @file  znet_bnotify.c
 *  @brief Обработка кадров-маяков 
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *      
 *      Обработка принятых кадров-маяков и обновление информации в таблице соседей.
 *  */

#include    <ztypes.h>
#include    <zcall.h>
#include    <zneighbor.h>
#include    <zmac_bnotify.h>
#include    <znet_attr.h>
#include    <znet_beacon.h>

/* Обработка принятого кадра маяка */
void    znet_beacon_notify( zcall_t     *zc )
{
    zargs_mbnotify_t    *arg = (zargs_mbnotify_t *)(zc->args);
    neighbor_t  *nbr_ptr = AFTER_TABLE;

    /* Если не совпадает идентификатор протокола, версия протокола или профиль стека,
     * то кадр маяк отбрасывается. */
    if( GET_PROTOCOL_VERSION(arg->bcn_payload->data) != nwkProtocolVersion ) 
        goto znet_bnotify_exit;   
    if( GET_PROTOCOL_ID(arg->bcn_payload->data) != 0 ) 
        goto znet_bnotify_exit;
    if( GET_STACK_PROFILE(arg->bcn_payload->data) != nwkStackProfile ) 
        goto znet_bnotify_exit;

    /* Поиск места в таблице соседей по адресу. 
     * Потенциально может появиться 2 записи одного узла в таблице соседей.
     * Если одна запись была расмещена по длинному адресу ( короткий был неизвестен ),
     * а другая по короткому адресу ( длинный не известен ). 
     * */
    if( arg->addr_mode == EXT_ADDR ) {
        /* Известен длинный адрес. Ищем место в таблице соседей по длинному адресу. */
        nbr_ptr = nbr_place_by_eaddr( &(arg->eaddr) );
        nbr_ptr->ext_addr = arg->eaddr;
        nbr_ptr->net_addr = (s_addr_t)-1;
    } else if( arg->addr_mode == SHORT_ADDR ) {
        /* Известен короткий адрес. Ищем место в таблице соседей по короткому адресу 
         * и идентификатору сети */
        nbr_ptr = nbr_place_by_saddr( arg->panid, arg->saddr );
        nbr_ptr->net_addr = arg->saddr;
        nbr_ptr->ext_addr = (e_addr_t)-1;
    };
    if( IN_TABLE(nbr_ptr) ) {
        /* Запись размещена в таблице соседей */
        // nbr_ptr->potential_parent = 1; /* Этот флаг должен устанавливаться после сканирования ( см. стр. 329) */
        /* Если узел разрешает присоединять к себе хотя бы один тип устройств, то выставляем флаг 
         * разрешения присоединения к узлу */
        if( GET_ROUT_CAPACITY(arg->bcn_payload->data)
                ||GET_END_CAPACITY(arg->bcn_payload->data) )
            nbr_ptr->permit_joining = arg->sf_spec.assoc_permit;
        else nbr_ptr->permit_joining = 0;
        /* Заполнение записи узла в таблице соседей данными из кадра-маяка этого узла */
        nbr_ptr->dev_type = (arg->sf_spec.pan_coord)?ZIGBEE_COORD:ZIGBEE_ROUTER;
        nbr_ptr->batt_life_ext = arg->sf_spec.batt_life_ext;
        nbr_ptr->relationship = NWK_STRANGER;
        nbr_ptr->channel = arg->channel;
        nbr_ptr->beacon_order = arg->sf_spec.beacon_order;
        nbr_ptr->sf_order = arg->sf_spec.sf_order;
        nbr_ptr->depth = GET_DEPTH(arg->bcn_payload->data);
        nbr_ptr->final_cap_slot = arg->sf_spec.final_cap_slot;
        nbr_ptr->s_panid = arg->panid;
        GET_EPANID( arg->bcn_payload->data, &(nbr_ptr->e_panid) );
        nbr_ptr->beacon_timestamp = arg->bcn_payload->time_stamp;
        GET_TX_OFFSET(arg->bcn_payload->data, &(nbr_ptr->beacon_offset) );
        nbr_ptr->lqi = arg->bcn_payload->lqi;
    }
znet_bnotify_exit:
    /* Если нет функции обратного вызова, то zcall_return удалит вызов, без 
     * удаления кадра-маяка. */
    if( zc->callback == 0 )
        zpkt_free(arg->bcn_payload ); 
    zcall_return(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_MAC_BEACON_NOTIFY, znet_beacon_notify );

