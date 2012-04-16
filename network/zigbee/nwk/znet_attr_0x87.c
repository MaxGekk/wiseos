//  vim:encoding=utf-8
/*! @file  znet_attr_0x87.c
 *  @brief Атрибут - таблица соседей
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <mem.h>
#include    <zigconf.h>
#include    <ztypes.h>
#include    <zneighbor.h>
#include    <znet_reset.h>

/* Таблица соседей содержит информацию об обнаруженных соседних узлах */
neighbor_t  nwkNeighborTable[ Z_NEIGHBOR_TABLE_SIZE ];

/* Сброс таблицы соседей. Заполняем нулями, исходя из предположения, что
 * если флаг занятости ( busy ) в нуле, значить запись не занята */
void    znet_reset_a0x87()
{
    mset( nwkNeighborTable, Z_NEIGHBOR_TABLE_SIZE*sizeof(neighbor_t),0);
}

REG_RESET( znet_reset_a0x87 );

