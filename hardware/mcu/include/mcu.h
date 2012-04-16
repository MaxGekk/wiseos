#ifndef   _MCU_H
#define   _MCU_H

/*! @file  mcu.h
 *  @brief Управление микроконтроллером
 *  @author     Max Gekk
 *  @author     Igor Pankratov
 *  @date       декабрь 2006
 *  @version    0.1 

 *  @defgroup hardware Взаимодействие с аппаратурой
 *  @defgroup mcu Управление микроконтроллером
 *  @ingroup  hardware
 *  @{ 
 *      Данный интерфейс предназначен для управления микроконтроллером.
 * */

void irq_enable();      /*!< Разрешение аппаратных прерываний */
void irq_disable();     /*!< Запрещение аппаратных прерываний */

void system_sleep();    /*!< Перевод аппаратной части в состояние сна */

/*! @} */

#endif   /* _MCU_H */

