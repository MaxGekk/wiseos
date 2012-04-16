//  vim:encoding=utf-8
/*! @file  znet_parent.c
 *  @brief Доступ к записи родителя в таблице соседей
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <znet_relative.h>

/* Получение указателя на запись родителя в таблице соседей */
neighbor_t*     get_parent()
{
    neighbor_t* nbr_ptr = nbr_next(0);  /* Получаем указатель на первую занятую запись */
    while( IN_TABLE( nbr_ptr) ) {       /* Пока nbr_ptr указывает на таблицу соседей */
        if( nbr_ptr->relationship == NWK_PARENT )   
            break;                      /* запись родительского узла найдена */
        nbr_ptr = nbr_next( nbr_ptr );  /* указатель на следующую занятую запись */
    }
    return nbr_ptr;
}

