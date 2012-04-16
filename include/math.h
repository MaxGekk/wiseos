#ifndef     _MATH_H
#define     _MATH_H

/*! @file  math.h
 *  @brief Описание интерфейса math
 *  @author     Max Gekk
 *  @date       сентябрь 2007
 *  @version    1

 *  @defgroup math Математические функции
 *  @ingroup  library
 *  @{ 
 * */

#include    <types.h>

/*! @fn uint16_t power( uint8_t a, uint8_t n );
 *  @brief Возведение a в степень n.
 *  @param a - основание степени
 *  @param n - показатель степени 
 *  @return Возвращается a в степени n.
 * */
uint16_t    power( uint8_t a, uint8_t n );

/*! @fn uint16_t    random8();
 *  @brief Получение случайного 8-битного числа 
 * */
uint16_t    random8();

/*! @fn uint16_t    random16();
 *  @brief Получение случайного 16-битного числа 
 * */
uint16_t    random16();

/*! @} */
#endif                          /*  _MATH_H   */

