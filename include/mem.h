#ifndef     _MEMORY_H
#define     _MEMORY_H
/*! @file  mem.h
 *  @brief Функции работы с памятью
 *  @author     Max Gekk
 *  @date       сентябрь 2007
 *  @version    0.1           

 *  @defgroup combuf Работа с памятью
 *  @ingroup  library
 *  @{
 *  */ 

#include   <types.h>

/*! @fn void    mset( void *dst, size_t len, uint8_t c );
 *  @brief Заполнение области памяти заданным значением
 *  @param dst - указатель на начало заполняемой области
 *  @param len - размер области в байтах
 *  @param c - значение, которым заполяется память
 * */
void    mset( void *dst, size_t len, uint8_t c );

/*! @fn void*   mcpy(void *restrict dst,const void *restrict src, size_t n);
 *  @brief  Копирование данных из одной области памяти в другую
 *  @param dst - указатель на начало области назначения
 *  @param src - указатель на начало области источника
 *  @param n - число копируемых байт
 *  @return - Возвращается указатель dst.
 * */
void*   mcpy(void *dst,const void *src, size_t n);

/*! @} */
#endif  /* _MEMORY_H */

