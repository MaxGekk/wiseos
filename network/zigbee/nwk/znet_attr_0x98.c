//  vim:encoding=utf-8
/*! @file  znet_attr_0x98.c
 *  @brief  Атрибут - версия протокола
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_const.h>
#include    <znet_reset.h>

/* Один стек может поддерживать несколько версий сетевого протокола ZigBee.
 * Данный атрибут определяет текущую версию протокола. 
 * */
uint8_t  nwkProtocolVersion = nwkcProtocolVersion;

/* Сброс атрибута в значение по умолчанию. */
void   znet_reset_a0x98()
{
    nwkProtocolVersion = nwkcProtocolVersion;
}

REG_RESET( znet_reset_a0x98 );

