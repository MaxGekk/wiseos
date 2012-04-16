//  vim:encoding=utf-8
/*! @file  znet_attr_0x96.c
 *  @brief  Атрибут - короткий адрес
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* По умолчанию широковещательный адрес. Такого адреса у узла быть не может. */
#define     DEFAULT_SHORT_ADDR  0xffff

/* Короткий (сетевой) адрес устройства */
uint16_t    nwkShortAddress = DEFAULT_SHORT_ADDR;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x96()
{
    nwkShortAddress = DEFAULT_SHORT_ADDR;
}

REG_RESET( znet_reset_a0x96 );


