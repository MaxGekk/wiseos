//  vim:encoding=utf-8
/*! @file  znet_attr_0x9a.c
 *  @brief  Атрибут - расширенный идентификатор сети
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */
#include    <ztypes.h>
#include    <znet_reset.h>

/* Значение по умолчанию */
#define     DEFAULT_EXT_PANID   0

/* Расширенный идентификатор сети */
uint64_t nwkExtendedPANID = DEFAULT_EXT_PANID;

/* Сброс атрибута в значение по умолчанию */ 
void   znet_reset_a0x9a()
{
    nwkExtendedPANID = DEFAULT_EXT_PANID;
}

REG_RESET( znet_reset_a0x9a );

