#ifndef     _COMBUF_H
#define     _COMBUF_H

/*! @file  combuf.h
 *  @brief Описание интерфейса combuf
 *  @author     Max Gekk
 *  @author     Igor Pankratov
 *  @date       декабрь 2006
 *  @version    0.1           

 *  @defgroup library Вспомогательные модули
 *  @defgroup combuf Коммуникационные буферы
 *  @ingroup  library
 *  @{ 
 *      Данный интерфейс предназначен для взаимодействия с внешней системой,
 *  с которой есть прямая связь ( com, usb, ... ). Взаимодействие происходит
 *  посредством коммуникационных буферов. 
 * */

#include    <types.h>

typedef int16_t combuf_t;       /*!< описатель буфера */

/*! @typedef combuf_type_t
 *  @brief Тип полезной нагрузки буфера
 * */
typedef enum {
    COMBUF_COMMAND = 0,         /*!< команда */
    COMBUF_DATA = 1,            /*!< буфер содержит данные, полученные из сети или
                                 *   предназначенные для отправки по сети */
    COMBUF_DEBUG = 2            /*!< отладочная информация */
} combuf_type_t;

typedef uint8_t combuf_size_t;  /*!< размер буфера */
typedef uint8_t combuf_pos_t;   /*!< позиция в буфере */

/*! @fn combuf_t    combuf_create( const uint8_t type, const combuf_size_t    payload_size )
 *  @brief Создание буфера.  
 *  @param type тип полезной нагрузки.
 *  @param payload_size размер полезной нагрузки.
 *  @return Функция возвращает описатель буфера ( >= 0 ) или ошибку ( < 0 ) в случае неудачного вызова.
 * */
combuf_t combuf_create(const uint8_t type, const combuf_size_t payload_size);

/*! @fn result_t    combuf_write( const combuf_t combuf,const combuf_pos_t  pos, const uint8_t  data  )
 *  @brief Запись данных в поле полезной нагрузки буфера.
 *  @param combuf Описатель буфера. Описатель возвращается при создании буфера.
 *  @param pos Позиция, в которую должны быть записаны данные.
 *  @param data Данные, которые должны быть записаны.
 * */
result_t combuf_write(const combuf_t combuf, const combuf_pos_t pos,
                      const uint8_t data);

/*! @fn uint8_t combuf_read( const   combuf_t    combuf, const   combuf_pos_t   pos )
 *  @brief Функция возвращает данные из определённой позиции полезной нагрузки буфера.
 *  @param combuf Описатель буфера. Описатель возвращается при создании буфера.
 *  @param pos Позиция из которой должны быть прочитаны данные.
 *  @return В случае возникновения ошибки функция возвращает отрицательное значение.
 * */
uint8_t combuf_read(const combuf_t combuf, const combuf_pos_t pos);

/*! @fn result_t    combuf_send( const  combuf_t    combuf )
 *  @brief Отправка полезной нагрузки буфера во внешнюю систему. 
 *  @param combuf Описатель буфера.
 * */
result_t combuf_send(const combuf_t combuf);

/*! @typedef result_t  (* combuf_recv_t )( combuf_type_t  type, combuf_size_t    payload_size )
 *  @brief Тип функции, которая будет вызвана после приёма буфера. Функция должна быть 
 *  зарегистрирована при помощи макроса REG_COMBUF_RECV.
 *  @param type тип полезной нагрузки буфера.
 *  @param size размер полезной нагрузки буфера.
 *  @return Возвращаемые значения:
 *              - ENOERR - буфер обработан функцией.
 *              - < 0 - в случае возникновения ошибки.
 *  @see REG_COMBUF_RECV            
 * */
typedef result_t(*combuf_recv_t) (combuf_type_t type,
                                  combuf_size_t payload_size);

/*! @def REG_COMBUF_RECV(fn)
 * Регистрация функции @a fn в качестве обработчика входных буферов.
 * */
#define REG_COMBUF_RECV(fn) combuf_recv_t __combufrecv_##fn __attribute__((unused,__section__ (".combufrecv"))) = fn;

/*! @} */
#endif                          /*  _COMBUF_H   */
