//  vim:encoding=utf-8
/*! @file  znet_attr_0x97.c
 *  @brief  Атрибут - профиль стека
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* Профиль стека по умолчанию. */
#define     DEFAULT_STACK_PROFILE   0

/* Об этом атрибуте мало что можно сказать.
 * Во всей спецификации ZigBee 2006 он встречается только один раз
 * в списке атрибутов.
 * */
uint8_t  nwkStackProfile = DEFAULT_STACK_PROFILE;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x97()
{
    nwkStackProfile = DEFAULT_STACK_PROFILE;
}

REG_RESET( znet_reset_a0x97 );


