//  vim:encoding=utf-8
/*! @file  znet_attr_0x8a.c
 *  @brief  Атрибут - марсимальное число запросов маршрута
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <types.h>
#include    <znet_const.h>	
#include    <znet_reset.h>

/* После первой неудачной попытки обнаружения маршрута, сетевой уровень
 * может попробовать ещё несколько раз. Максимальное число этих дополнительных
 * попыток задаётся этим атрибутом. */
uint8_t  nwkRouteDiscoveryRetriesPermitted = nwkcDiscoveryRetryLimit;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x8a()
{
    nwkRouteDiscoveryRetriesPermitted = nwkcDiscoveryRetryLimit;
}

REG_RESET( znet_reset_a0x8a );

