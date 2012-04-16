//  vim:encoding=utf-8
/*! @file  znet_child.c
 *  @brief Доступ к дочерним записям
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1
 *      Поиск заиписей о дочерних узлах в таблице соседей.            
 *  */

#include    <znet_relative.h>

/* Получение указателя на следующую запись о дочернем узле.
 * Возвращается указатель как на реальных детей, так и на бывших. 
 * Хотя бывших детей не бывает. Только если у них нет имени, то есть
 * адреса. */
neighbor_t*     child_next( neighbor_t    *nbr_ptr )
{
    nbr_ptr = nbr_next( nbr_ptr );  /* Получение указателя на следующую занятую запись */
    while( IN_TABLE( nbr_ptr) ) {   /* Пока nbr_ptr указывает не таблицу соседей */
        if( (nbr_ptr->relationship == NWK_CHILD)    /* Реальный ребёнок */
         /* Бывший ребёнок, но с адресом. */
         ||(( nbr_ptr->relationship == NWK_PREVIOUS_CHILD)&&(nbr_ptr->net_addr != 0xFFFF)) )             
            break;
        nbr_ptr = nbr_next( nbr_ptr );  /* Следующая занятая запись */  
    }
    return nbr_ptr;
}

/* Сколько у нас детей? */
uint16_t    child_count()
{
    uint16_t count = 0;     /* Счётчик детей */
    neighbor_t  *child_ptr = child_next(0);     /* Указатель на первого ребёнка */
    while( IN_TABLE( child_ptr ) ) {
        /* Очередной ребёнок обнаружен */
        ++count;
        child_ptr = child_next( child_ptr );    /* Получаем указатель на следующего ребёнка */
    }
    return count;
}

/* Поиск ребёнка, удовлетворяющего определённому условию  */
#define     CHILD_BY( condition ) \
{ \
    neighbor_t  *child_ptr = child_next(0); /* Итератор по детям. Инициализируем первым ребёнком. */ \
    while( IN_TABLE( child_ptr ) ) { /* Пока итератор указывает на таблицу соседей, продолжаем перебор детей */ \
        if( condition ) /* Условие, по которому фильтруем детей */\
            break;\
        child_ptr = child_next( child_ptr );    /* Следующий ребёнок */ \
    } \
    return child_ptr; \
}

/* Поиск ребёнка с определённым коротким адресом. */
neighbor_t*     child_by_addr( s_addr_t  addr )
{
    /* Ищем только по короткому адресу, без использования идентификатора сети,
     * так как мы ищем среди своих детей */
    CHILD_BY( child_ptr->net_addr == addr );
}

/* Поиск ребёнка с определённым длинным адресом */
neighbor_t*     child_by_eaddr( e_addr_t  addr )
{
    CHILD_BY( child_ptr->ext_addr == addr );
}

