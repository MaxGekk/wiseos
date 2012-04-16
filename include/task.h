#ifndef   _TASK_H
#define   _TASK_H

#include <types.h>

/*! @file   task.h
 *  @brief  Управление задачами.
 *  @author Max Gekk
 *  @author Igor Pankratov
 *  @date   декабрь 2006
 *  @version 0.1 

 *  @defgroup task Системные задачи
 *  @ingroup  kernel
 *  @{ 
 *      Интерфейс управления задачами.
 * */

/*! @typedef void (* task_func_t )()
 *  @brief Тип функции-задачи.
 * */
typedef void (*task_func_t) ();

extern const task_func_t __task_start;
extern const task_func_t __task_end;

/*! @def TASK(fn,priority)
 *  @brief Регистрация функции в качестве задачи.
 *  @param fn - регистрируемая функция
 *  @param priority - приоритет вызова функции @a fn. Чем меньше лексикографическое значение @a priority, тем
 *                    раньше будет вызвана функция @a fn.
 * */
#define TASK(fn,priority) static task_func_t __task_##fn __attribute__ ((unused,__section__ ("." #priority ".tasks"))) = fn;

result_t post_task(const uint16_t task_offset);

/*! @def POST_TASK(fn)
 *  @brief Постановка задачи в очередь на выполнение.
 *  @param fn - функция-задача.
 * */
#define POST_TASK(fn) post_task( ( &__task_##fn - &__task_start ) )

/*! @} */

#endif                          /* _TASK_H */
