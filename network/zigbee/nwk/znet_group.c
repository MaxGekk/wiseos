//  vim:encoding=utf-8
/*! @file  znet_group.c
 *  @brief Операции с группами
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1   
 *
 *      Модуль предоставляет реализацию функций работы с группами.
 *  Включение узла в группу, исключение узла из группы. ( см. 3.7.6 ).        
 *  */

#include    <debug.h>
#include    <zigconf.h>
#include    <znet_group.h>
#include    <znet_reset.h>

typedef  uint16_t busy_map_t;
static busy_map_t busy_map;     /* Битовая маска занятых элементов в массиве 
                                   идентификторов групп - groups*/

/* Макрос возвращающий 1, если достигнуто максимальное число групп, 
 * в которые может входить узел */
#define   GRP_TABLE_FULL  (busy_map == ((busy_map_t)-1))

/* Максимальное число групп, в которые реально может входить узел. */
#define  MAX_GRP \
    ((8*sizeof(busy_map)<Z_TOTAL_GROUP)?8*sizeof(busy_map):Z_TOTAL_GROUP)

/* Массив идентификаторов групп, в которые входит данный узел */
uint16_t    groups[ MAX_GRP ];

/* Включить узел в группу */
result_t    zgroup_add( uint16_t    group_id )
{
    uint16_t i;
    busy_map_t  mask;
    if( GRP_TABLE_FULL )
        return ENOMEM;
    /* Последовательно перебираем все биты битовой маски групп в поиске 
     * свободной позиции в массиве групп.*/
    for( mask = 1,i=0; i<MAX_GRP; i++,mask <<= 1 ) 
        if( (busy_map & mask) == 0 ) {
            /* Если свободная позиция найдена, то занимаем её. */
            groups[i] = group_id;
            busy_map |= mask;
            return ENOERR;
        }
    ASSERT( MAX_GRP <= i, "Warning: This can't happen! "); 
    return ENOMEM;
}

/* Определение того, что узел входит в указанную группу.
 * Функция с побочным эфектом: через m и i возвращает маска группы и
 * позиция в массиве групп.  */
static result_t    _zgroup_find( uint16_t   group_id,
       busy_map_t   *const m, uint16_t *const i  )
{
    for( *m = 1,*i=0; *i<MAX_GRP; (*i)++,*m <<= 1 ) 
        if( (busy_map & *m) && ( groups[*i] == group_id ) ) {
            return ENOERR;
        }
    return ENOTFOUND;
}

/* Исключение узла из группы */
result_t    zgroup_del( uint16_t    group_id )
{
    result_t res;
    uint16_t i;
    busy_map_t  mask;

    /* Проверяем входит ли узел в группу с идентификатором group_id,
     * если да, исключаем его. */
    res = _zgroup_find(group_id, &mask, &i);
    if( IS_OK(res) )
        busy_map &= ~mask;
    return res;
}

/* Обёртка к функции _zgroup_find. Предоставляется пользователю. */
result_t    zgroup_find( uint16_t   group_id )
{
    uint16_t i;
    busy_map_t  mask;

    return _zgroup_find(group_id, &mask, &i);
}

/* Сброс модуля: исключение узла из всех групп */
void    znet_reset_groups()
{
    busy_map = (busy_map_t)0;
}

REG_RESET( znet_reset_groups );

