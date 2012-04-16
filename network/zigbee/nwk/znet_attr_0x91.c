//  vim:encoding=utf-8
/*! @file  znet_attr_0x91.c
 *  @brief  Атрибут определяющий тип маршрутизации
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* По умолчанию используется древовидная маршрутизация */
#define     DEFAULT_USE_TREE_ROUTING    TRUE

/* Если атрибут равен TRUE, то использовать древовидную 
 * маршрутизацию, иначе динамическую табличную. */
bool_t nwkUseTreeRouting = DEFAULT_USE_TREE_ROUTING;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x91()
{
    nwkUseTreeRouting = DEFAULT_USE_TREE_ROUTING;
}

REG_RESET( znet_reset_a0x91 );


