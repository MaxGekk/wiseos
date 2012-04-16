//  vim:encoding=utf-8
/*! @file  znet_attr_0x88.c
 *  @brief Атрибут - время охвата сети
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <types.h>
#include    <znet_reset.h>

/* Значение атрибута по умолчанию */
#define     DEFAULT_NBDELIVERY_TIME     9

/* Время ( в секундах ) необходимое для охвата сети широковещательным сообщением */
uint8_t  nwkNetworkBroadcastDeliveryTime = DEFAULT_NBDELIVERY_TIME;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x88()
{
    nwkNetworkBroadcastDeliveryTime = DEFAULT_NBDELIVERY_TIME;
}

REG_RESET( znet_reset_a0x88 );

