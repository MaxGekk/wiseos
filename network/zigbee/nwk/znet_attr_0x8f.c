//  vim:encoding=utf-8
/*! @file  znet_attr_0x8f.c
 *  @brief  Атрибут с информацией об узле
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <types.h>
#include    <ztypes.h>
#include    <znet_reset.h>

/* По умолчанию все поля обнулены */
#define     DEFAULT_CAPABILITY_INFO  \
{ .alt_coord = 0, .dev_type = 0, .pow_src = 0, \
  .rx_on_when_idle = 0, .sec = 0, .alloc_addr = 0 \
}

/* Атрибут является структурой и содержит информацию о возможностях узла. */
cap_info_t  nwkCapabilityInformation = DEFAULT_CAPABILITY_INFO;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x8f()
{
    const cap_info_t default_capability = DEFAULT_CAPABILITY_INFO;
    nwkCapabilityInformation = default_capability;
}

REG_RESET( znet_reset_a0x8f );

