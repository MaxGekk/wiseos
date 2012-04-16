//  vim:encoding=utf-8
/*! @file  znet_attr_0x94.c
 *  @brief  Атрибут - адресный инкремент
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* По умолчанию следующий выделенный адрес больше предыдущего на 1. */
#define     DEFAULT_ADDR_INC    0x0001

/* Атрибут задаёт шаг, с которым выделяются адреса дочерним узлам, по схеме
 * раздачи адресов, управляемой верхним уровнем. */
uint16_t    nwkAddressIncrement = DEFAULT_ADDR_INC;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x94()
{
    nwkAddressIncrement = DEFAULT_ADDR_INC;
}

REG_RESET( znet_reset_a0x94 );

