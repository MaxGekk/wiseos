//  vim:encoding=utf-8
/*! @file  znet_attr_0x8e.c
 *  @brief  Атрибут - симметричны ли пути
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <types.h>
#include    <znet_reset.h>

/* По умолчанию пути не семметричны */
#define     DEFAULT_SYM_LINK  FALSE

/* Если обнаружен путь в одну сторону, то можно ли считать, что обнаружен
 * и путь в обратную сторону. Если атрибут равен TRUE, то да, иначе нет. */
bool_t  nwkSymLink = FALSE;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x8e()
{
    nwkSymLink = FALSE;
}

REG_RESET( znet_reset_a0x8e );

