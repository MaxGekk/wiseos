//  vim:encoding=utf-8
/*! @file  neighbor.c
 *  @brief Реализация интерфейса доступа к таблице соседей
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <debug.h>
#include    <zneighbor.h>

#define     IS_FREE( nbr_ptr )  ( 0 == nbr_ptr->busy )
#define     IS_BUSY( nbr_ptr )  (!IS_FREE( nbr_ptr ))

/* Проверка корректности указателя на запись в таблице соседей  */
#define CHECK_PTR( nbr_ptr ) \
    /* Проверка того, что указатель в пределах таблицы соседей */ \
    ASSERT1( ( nbr_ptr < nwkNeighborTable )|| !IN_TABLE(nbr_ptr), \
            "Invalid nbr_ptr [%x] : it is out of neighbor table", (uint16_t)nbr_ptr );\
    /* Проверка того, что указатель на одину из записей */ \
    ASSERT1( (nbr_ptr - nwkNeighborTable) % sizeof(neighbor_t), \
            "Invalid nbr_ptr [%x] : it is not point to neighbor record", (uint16_t)nbr_ptr );

/* Поиск следующей занятой записи в таблице соседей */
neighbor_t* nbr_next( neighbor_t    *nbr_ptr )
{
    /* Поиск начинается с записи следующей за той, на которую указывает
     * nbr_ptr. Если nbr_ptr равно 0, то начинаем поиск с начала таблицы */
    if( nbr_ptr != (neighbor_t  *)0 ) {
        CHECK_PTR( nbr_ptr );
        nbr_ptr++;
    } else
        nbr_ptr = nwkNeighborTable;

    while( IN_TABLE(nbr_ptr) && IS_FREE(nbr_ptr) )
        nbr_ptr++;

    return nbr_ptr;
}

#define     NBR_PLACE_BY( condition ) \
{ \
    neighbor_t  *free_nbr_ptr = AFTER_TABLE; /* Указатель на нужную запись*/ \
    neighbor_t  *nbr_ptr; \
\
    for( nbr_ptr = nwkNeighborTable; IN_TABLE( nbr_ptr ); nbr_ptr++ ) { \
        if( IS_BUSY( nbr_ptr ) ) { \
            /* В процессе поиска удаляем все записи, для которых condition истино */ \
            if( condition ) { \
                nbr_ptr->busy = 0; \
                free_nbr_ptr = nbr_ptr; /* Запоминаем позицию на последнюю запись,  \
                    удовлетворяющую условию condition */ \
            } \
        } else if( !IN_TABLE( free_nbr_ptr ) ) /* Если до этого не была найдена ни одна запись,  \
            удовлетворяющая условию, то запоминаем последнюю из обнаруженных свободных записей */ \
            free_nbr_ptr = nbr_ptr; \
    } \
    /* Если была найдена хоть одна запись, удовлетворяющая условию, или свободная запись, то возвразаем \
     * в качестве результата указатель на неё. */ \
    if( IN_TABLE(free_nbr_ptr) ) \
        free_nbr_ptr->busy = 1; \
    return free_nbr_ptr; \
}

/* Выделение места для записи с длинным адресом, который содержится по
 * указателю e_addr_ptr.*/
neighbor_t*     nbr_place_by_eaddr( const e_addr_t  *const e_addr_ptr )
{
    ASSERT( 0==e_addr_ptr,"Error: invalid argument e_addr_ptr == 0");
    
    NBR_PLACE_BY( *e_addr_ptr == nbr_ptr->ext_addr );
}

/* Выделение места для записи с заданным коротким адресом и коротким идентификатором сети */
neighbor_t*     nbr_place_by_saddr( const s_panid_t s_panid, const s_addr_t  s_addr )
{
    NBR_PLACE_BY( (s_addr == nbr_ptr->net_addr)&&(s_panid == nbr_ptr->s_panid) );
}

/* Очистка всей таблицы соседей */
void    nbr_clean()
{
    neighbor_t  *nbr_ptr;

    for( nbr_ptr = nbr_next(0); IN_TABLE(nbr_ptr) ; nbr_ptr = nbr_next( nbr_ptr ) )
        nbr_ptr->busy = 0;
}

