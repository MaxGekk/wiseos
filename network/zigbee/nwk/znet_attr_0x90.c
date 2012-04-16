//  vim:encoding=utf-8
/*! @file  znet_attr_0x90.c
 *  @brief  Атрибут определяющий механизм раздачи адресов
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* По умолчанию использовать распределённую древовидную раздачу адресов */
#define     DEFAULT_USE_TREE_ADDR_ALLOC     TRUE

/* Атрибут определяющий раздачу адресов дочерним узлам:
 * TRUE - использовать древовидную раздачу адресов,
 * FALSE - алгоритм раздачи адресов определяется вышележащим уровнем  */
bool_t  nwkUseTreeAddrAlloc = DEFAULT_USE_TREE_ADDR_ALLOC; 

/* Сброс атрибута в значение по умолчанию */
void znet_reset_a0x90()
{
    nwkUseTreeAddrAlloc = DEFAULT_USE_TREE_ADDR_ALLOC; 
}

REG_RESET( znet_reset_a0x90 );

