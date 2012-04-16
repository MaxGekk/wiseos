/*! @file   critical.c
 *  @brief  Реализация критических секций
 *  @author     Max Gekk
 *  @version    0.1
 *  @date   декабрь 2006 г.
 * */

#include <types.h>   
#include <critical.h>
#include <mcu.h>
#include <debug.h>

#define MAX_CRITICAL_COUNTER 0xffff /*!< Максимальное число вхождений в критические секции */
#define MIN_CRITICAL_COUNTER 0  /*!< Начальное значение счётчика вхождений в критические секции */

/*! @var count
 *  @brief Счётчик вхождений в критические секции.
 *
 *  Счётчик используется по следующим причинам:
 *      - Если одна критическая секция вложена в другую, то
 *      при отсутствии счётчика прерывания разрешались бы выходе
 *      из вложенной секции.
 *      - Контроллируется соответствие числа входов и выходов из
 *      критических секций.
 *      - Обнаружение зацикливания при вхождении в критические секции.
 * */
uint16_t count = MIN_CRITICAL_COUNTER;

void critical_enter()
{
    irq_disable();
    if (MAX_CRITICAL_COUNTER == count) {
        DBG("Critical section re-enter counter overflow");
        return;
    }
    ++count;
}

void critical_exit()
{
    if (MIN_CRITICAL_COUNTER == count) {
        DBG("Critical section exit without enter");
        return;
    }
    --count;
    if (MIN_CRITICAL_COUNTER == count)
        irq_enable();
}
