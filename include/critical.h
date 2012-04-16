#ifndef   _CRITICAL_H
#define   _CRITICAL_H

/*! @file   critical.h
 *  @brief  Интерфейс критических секций
 *  @author Max Gekk
 *  @author Igor Pankratov
 *  @date   декабрь 2006
 *  @version 0.1 

 *  @defgroup critical Критические секции
 *  @ingroup  kernel
 *  @{ 
 *      Данный интерфейс реализует механизм критических секций.
 *  Критическая секция - это блок кода, внутри которого отключены аппаратные прерывания.
 *  Время нахождения в критической секции должно быть сведено к минимуму. Допустимы
 *  вложенные критические секции.
 * */

/*! @fn void critical_enter()
 *  @brief Вход в критическую секцию.
 * */
void critical_enter();

/*! @fn void critical_exit()
 *  @brief Выход из критической секции.
 * */
void critical_exit();

/*! @def CRITICAL_BLOCK( block )
 *  @brief Размещение блока кода @a block в критической секции.
 * */
#define CRITICAL_BLOCK( block )   critical_enter(); { block;} critical_exit();

/*! @}*/
#endif                          /* _CRITICAL_H */
