//  vim:encoding=utf-8
/*! @file  znet_attr_0x93.c
 *  @brief  Атрибут - число доступных адресов
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* Число доступных адресов по умолчанию */
#define     DEFAULT_AVAIL_ADDR  0x0000

/* Число оставшихся коротких адресов, которые можно выделить дочерним узлам */
uint16_t    nwkAvailableAddress = DEFAULT_AVAIL_ADDR;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x93()
{
    nwkAvailableAddress = DEFAULT_AVAIL_ADDR;
}

REG_RESET( znet_reset_a0x93 );


