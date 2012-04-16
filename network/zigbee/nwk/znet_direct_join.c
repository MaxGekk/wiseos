//  vim:encoding=utf-8
/*! @file  znet_direct_join.c
 *  @brief Реализация прямого присоединения
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *
 *      Прямое присоединение означает непосредственное добавление вешележащим
 *  уровенем информации о дочернем узле в таблицу соседей. Подробности можно
 *  найти в разделах 3.3.7 и 3.7.1.3.3 спецификации ZigBee.
 *
 *  */

#include    <ztypes.h>
#include    <zigconf.h>
#include    <zcall.h>
#include    <zneighbor.h>
#include    <znet_attr.h>
#include    <zmac_attr.h>
#include    <znet_join.h>
#include    <znet_addr.h>
#include    <znet_beacon.h>
#include    <znet_relative.h>

/* Обработка запроса на добавление нового дочернего узла */
void    net_direct_join( zcall_t    *zc )
{
    neighbor_t  *nbr_ptr;
    zargs_djoin_t   *arg = (zargs_djoin_t *)(zc->args);

    /* Ищем место в таблице соседей */
    nbr_ptr = nbr_place_by_eaddr( &(arg->dev_addr) );
    if( IN_TABLE(nbr_ptr) ) {
        /* Найдено место в таблице соседей для нового дочернего узла */
        if( nbr_ptr->ext_addr != arg->dev_addr ) {
            /* Для нового узла выделена новая запись в таблице соседей */
            uint16_t  total_child;

            nbr_ptr->relationship = NWK_CHILD;
            total_child = child_count();

            /* Проверяем не превышен ли максимальный порог */
            if( total_child <= nwkMaxChildren ) {
                /* Выделяем новый короткий адрес для нового дочернего узла */
                s_addr_t addr = znet_addr_alloc( arg->cap_info.dev_type );
                if( 0xffff != addr ) {
                    /* Адрес успешно выделен. Заполняем в таблице соседей поля записи нового дочернего узла */
                    nbr_ptr->rx_on_when_idle = arg->cap_info.rx_on_when_idle;
                    nbr_ptr->potential_parent = 0;
                    nbr_ptr->permit_joining = arg->cap_info.alloc_addr;
                    nbr_ptr->dev_type = (arg->cap_info.dev_type == FFD)? ZIGBEE_ROUTER : ZIGBEE_ENDDEV ;
                    nbr_ptr->depth = nwkExtraAttr.depth;
                    nbr_ptr->net_addr = addr;
                    nbr_ptr->ext_addr = arg->dev_addr;
                    nbr_ptr->e_panid = nwkExtendedPANID;    
                    if( total_child == nwkMaxChildren ) {
                        /* Больше детей мы присоединить не можем. Говорим это другим узлам через 
                         * кадр-маяк, сбрасывая биты разрешения присоединения как для роутеров, так и
                         * для конечных устройств. */
                        bcn_all_cap_off();
                    }
                    arg->status = SUCCESS;
                    zcall_return(zc);
                    return;
                } else {
                    /* Не получилось выделить адрес для данного типа устройства.
                     * Сбрасываем в кадре маяке соответствующий бит разрешения присоединения для этого
                     * типа устройств.
                     * */
                    bcn_cap_off( arg->cap_info.dev_type );
                    arg->status = MAC_NO_SHORT_ADDRESS;
                }
            } else {
                /* Превышен максимально допустимый порог для числа дочерних узлов */
                arg->status = MAC_NO_SHORT_ADDRESS;       
                bcn_all_cap_off();
            }
            /* Удаляем выделеную новую запись для нового дочернего узла из таблицы соседей */
            nbr_ptr->busy = 0;
        } else {
            /* Информация о узле уже есть в таблице соседей */
            arg->status = NWK_ALREADY_PRESENT;
        }
    } else {
        /* В таблице соседей нет места для нового дочернего узла */
        arg->status = NWK_NEIGHBOR_TABLE_FULL;
    }
    zcall_return(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_NLME_DIRECT_JOIN, net_direct_join );

