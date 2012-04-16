#ifndef     _UART_H
#define     _UART_H

/*! @defgroup uart Приём/передача по UART
 *  @ingroup  hardware
 *  @{ 
 *  @file  uart.h
 *  @brief      Взаимодействие по UART
 *  @author     Max Gekk
 *  @author     Igor Pankratov
 *  @date       декабрь 2006
 *  @version    0.1 
 * */

#include    <types.h>

void    uart_start();       /*!< Запуск UART */
void    uart_stop();        /*!< Остановка UART */

/*! @fn void    uart_tx( const uint8_t  data )
 *  @brief  Отправка одного байта по UART
 *  @param  data - отправляемые данные
 * */
void    uart_tx( const uint8_t  data ); 
void    uart_tx_done();     /*!< Извещение об окончании отправки данных по UART */

/*! @fn void    uart_rx_done( uint8_t    data )
 *  @brief  Уведомление о приёме одного байта по UART
 *  @param  data - принятые данные
 * */
void    uart_rx_done( uint8_t    data );

/*! @} */
#endif    /* _UART_H */

