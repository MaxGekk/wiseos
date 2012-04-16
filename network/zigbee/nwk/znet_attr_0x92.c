//  vim:encoding=utf-8
/*! @file  znet_attr_0x92.c
 *  @brief  Атрибут - следующий адрес
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* Следующий адрес по умолчанию */
#define     DEFAULT_NEXT_ADDRESS    0x0000

/* Адрес, который должен быть выделен устройству, запросившему ассоциацию. 
 * Атрибут используется только в механизме выделения адресов, управляемым 
 * вышележащим уровнем ( то есть при nwkUseTreeAddrAlloc равном FALSE ).
 * */
uint16_t    nwkNextAddress = DEFAULT_NEXT_ADDRESS;

/* Сброс атрибута в значение по умолчанию  */
void    znet_reset_a0x92()
{
    nwkNextAddress = DEFAULT_NEXT_ADDRESS;
}

REG_RESET( znet_reset_a0x92 );

