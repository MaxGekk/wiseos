//  vim:encoding=utf-8
/*! @file  znet_attr_0x86.c
 *  @brief Атрибут - максимальное число маршрутизаторов
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* Максимальное число маршрутизаторов 5. Так максимальное
 * число всех детей 5 */
#define     DEFAULT_MAX_ROUTERS     0x05

/* Максимальное число маршрутизаторов среди всех детей */
uint8_t  nwkMaxRouters = DEFAULT_MAX_ROUTERS;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x86()
{
    nwkMaxRouters = DEFAULT_MAX_ROUTERS;
}

REG_RESET( znet_reset_a0x86 );

