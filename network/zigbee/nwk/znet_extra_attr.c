//  vim:encoding=utf-8
/*! @file  znet_extra_attr.c
 *  @brief Дополнительные атрибуты сетевого уровня
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */
#include    <ztypes.h>
#include    <znet_attr.h>
#include    <znet_reset.h>

/* Дополнительные атрибуты:
 * type - тип ZigBee устройства в данный момент
 * is_joined - присоединено ли устройство к сети
 * channel - номер канала
 * depth - глубина устройства в сети относительно ZigBee-координатора.
 * */
nwk_extra_attr_t  nwkExtraAttr;

/* Сброс дополнительных атрибутов */
void    znet_reset_extra()
{
    nwkExtraAttr.type = ZIGBEE_INVDEV;
    nwkExtraAttr.is_joined = 0;
    nwkExtraAttr.channel = 0;
    nwkExtraAttr.depth = 15;
}

REG_RESET( znet_reset_extra );

