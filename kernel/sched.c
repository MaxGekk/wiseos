/*! @file   sched.c
 *  @brief  Реализация планировщика задач и загрузчика
 *  @author     Max Gekk
 *  @version    0.1
 *  @date   ноябрь 2006 г.

 *  @defgroup sched Планировщик задач
 *  @ingroup task
 *  @{
 *  */

#include    <types.h>
#include    <mcu.h>
#include    <debug.h>
#include    <critical.h>
#include    <task.h>
#include    <init.h>
#include    <irq.h>

extern const irq_handler_t __irq_start; /*!< Указатель на первую функцию-обработчик прерывания */

extern init_func_t __init_start;    /*!< Указатель на первую функцию-инициализатор */
extern init_func_t __init_end;  /*!< Указатель на последнюю функцию-инициализатор */

typedef uint16_t task_bitmap_t; /*!< Тип битового массива задач */

/*! @var task_bitmap
 *  @brief Битовый массив запущенных задач.
 *  Задача стоит в очереди на выполнение, если значение соответствующего задаче бита равно 1.
 *  i-ый бит в битовом массиве соответствует задаче-функции, адрес которой смещён относительно
 *  __task_start на i*sizeof(*void) байт.
 * */
static task_bitmap_t task_bitmap = 0;

/*! @fn result_t post_task( const uint16_t task_offset )
 *  @brief Поместить задачу в очередь на выполнение.
 *  @param task_offset - смещение адреса задачи относительно __task_start
 * */
result_t post_task(const uint16_t task_offset)
{
    const task_bitmap_t mask = (1 << task_offset);
    CRITICAL_BLOCK(task_bitmap |= mask);
    return ENOERR;
}

/*! @fn  static inline uint16_t search_posted_task()
 *  @brief Поиск задачи готовой к выполнению 
 *  @result смещение задачи с наивысшим приоритетом и готовой к выполнению.
 * */
static inline uint16_t search_posted_task()
{
    uint16_t _task_bitmap;
    uint16_t tnum = 0;

    CRITICAL_BLOCK(_task_bitmap = task_bitmap - 1;
                   task_bitmap &= _task_bitmap; _task_bitmap ^= task_bitmap;);
    while (_task_bitmap != 0) {
        tnum++;
        _task_bitmap >>= 1;
    }
    return tnum;
}

/*! @fn static inline void run_scheduler()
 *  @brief Запуск задачи с наивысшим приоритетом готовой к выполнению. 
 *  Если такая задача не обнаруживается, то система переходит
 *  в состояние сна. Выход из состояния сна возможен только по
 *  аппаратному прерыванию.
 * */
static inline void run_scheduler()
{
    if (task_bitmap) {
        task_func_t *task_func = &__task_start;
        task_func += search_posted_task();
        (*task_func) ();
    } else
        system_sleep();
}

uint8_t system_running = 1;     /*!< Флаг запуска системы */

int _main_()
{
    {                           /* Проверка того, что значения __task_start и __task_end корректны и 
                                   число задач не превышает размер битового массива задач. */
        int16_t task_num = &__task_end - &__task_start;

        if (task_num <= 0) {
            DBG("task list is empty");
            return -1;
        } else if (8 * sizeof(task_bitmap) < task_num) {
            DBG("task list is big");
            return -2;
        }
    }
    irq_enable();
    {                           /* Инициализация системы */
        init_func_t *init_func = &__init_start;
        for (; init_func < &__init_end; init_func++)
            (*init_func) ();
    }
    while (system_running)
        run_scheduler();

    return 0;
}

int main() { return _main_(); }

void    process_irq( uint16_t irq_num )
{
    const irq_handler_t *irq_handler = &__irq_start;
    irq_handler += irq_num;
    if( EXISTS( *irq_handler ) )
        (*irq_handler)();
    return;
}

/*! @} */
