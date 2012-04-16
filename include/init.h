#ifndef _INIT_H
#define _INIT_H

/*! @file   init.h
 *  @brief  Инициализация модулей.
 *  @author Max Gekk
 *  @author Igor Pankratov
 *  @date   декабрь 2006
 *  @version 0.1 

 *  @defgroup kernel Ядро ОС
 *  @defgroup init Инициализация
 *  @ingroup  kernel
 *  @{ 
 *      Интерфейс предоставляет макросы для регистрации функций инициализации модулей системы.
 * */

/*! @typedef void (* init_func_t )()
 *  @brief Тип функции - инициализатора модуля.
 * */
typedef void (*init_func_t) ();

/*! @def PRIORITY_INIT(fn,priority)
 *  @brief Регистрация функции в качестве инициализатора модуля. Функция вызывается при запуске системы.
 *  @param fn - регистрируемая функция
 *  @param priority - приоритет вызова функции @a fn. Чем меньше лексикографическое значение @a priority, тем
 *                    раньше будет вызвана функция @a fn.
 * */
#define PRIORITY_INIT(fn,priority) init_func_t __init_##fn __attribute__((unused,__section__ ("." #priority ".initos"))) = fn;

/*! @def INIT(fn)
 *  @brief Регистрация функции в качестве инициализатора. Функция вызывается при запуске системы.
 *  @param fn - регистрируемая функция
 * */
#define INIT(fn) PRIORITY_INIT(fn, A);

/*! @} */
#endif                          /*INIT_H */
