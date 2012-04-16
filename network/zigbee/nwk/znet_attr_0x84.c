//  vim:encoding=utf-8
/*! @file  znet_attr_0x84.c
 *  @brief Атрибут - максимальное число детей
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */


#include    <ztypes.h>
#include    <znet_reset.h>

/* Максимальное число детей по умолчанию 7.
 * При этом максимальная глубина сети, если все дети
 * маршрутизаторы, равна 5.
 * */
#define     DEFAULT_MAX_CHILDREN    0x07

/* Максимальное число дочерних узлов ( как маршрутизаторов, так и
 * конечных узлов */
uint8_t  nwkMaxChildren = DEFAULT_MAX_CHILDREN;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x84()
{
    nwkMaxChildren = DEFAULT_MAX_CHILDREN;
}

REG_RESET( znet_reset_a0x84 );

