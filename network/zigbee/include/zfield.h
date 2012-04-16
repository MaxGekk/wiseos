//  vim:encoding=utf-8
#ifndef     _ZIGBEE_FIELD_H
#define     _ZIGBEE_FIELD_H
/*! @file  zfield.h
 *  @brief Интерфейс доступа к полям фрейма
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup ZIGBEE_FIELD Доступ к полям фрейма
 *  @ingroup  ZIGBEE
 *  @{
 *      Данный интерфейс позволяет обращаться к полям фреймов
 *  стандарта IEEE 802.15.4 и ZigBee независимо от ограничений аппаратуры.
 */

/*! @typedef field_info_ft
 *  @brief Тип функции, возвращающей по порядковому номеру поля его размер
 *  и указатель на него.
 * */
typedef     void*    (* field_info_ft )( uint8_t *frame, 
        uint8_t num, uint16_t* len_ptr );

#if     !defined( Z_FIELD_BUF_SIZE )
#define     Z_FIELD_BUF_SIZE    8   /* Размер внутреннего буфера поля */
#endif

/* Длина поля переменного размера */
#define     ZNET_VAR_LEN    0xff

/* Структура, содержащая информацию о поле фрейма */
typedef struct {
    uint8_t     num;        /*!< Порядковы номер поля в общей структуре фрема. Нумерация начинается с 0. */
    uint8_t     is_buf;     
    uint16_t    len;        /*!< Длина поля в байтах */
    void*       ptr;        
    void*       frame;      /*!< Указатель на начало фрейма */
    field_info_ft   info_func;  /*!< Функция, возвращающая по номеру поля его размер и указатель на него */
    uint8_t     buf[ Z_FIELD_BUF_SIZE ];        
} field_t;

/* Инициализация поля */
#define     FIELD_INIT( fp, fr, fn ) \
    { (fp)->frame = fr, (fp)->info_func = fn;}

/*! @fn void*   zfield_find( field_t* fp, uint16_t num );
 *  @brief Поиск информации о поле.
 *      Вызывается функция info_func, которая возвращает информацию о поле.
 *  Поэтому перед вызовов zfield_find() поле должно быть проинициализировано с помощью
 *  FIELD_INIT. 
 *  @param fp - указатель на структуру, описывающую поле
 *  @param num - порядковый номер поле. Нумерация начинается с нуля. Порядковый номер поля соответствует
 *  его последовательному номеру в наиболее общем представлении фрейма.
 *  @return В случае успеха возвращает указатель на тело поля. Только по этому указателю можно работать
 *  с телом поля. В случае ошибки возвращается 0.
 * */
void*   zfield_find( field_t* fp, uint16_t num );

/*! @fn result_t    zfield_unpack( field_t* fp );
 *  @brief Распаковка поля во фрейм
 *      Перед вызовов данной функции обязательно должен производится поиск поля с
 *  помощью функции zfield_find().
 *  @param fp - указатель на структуру, описывающую поле
 *  @return В случае успеха возвращается ENOERR, иначе значение <0.
 * */
result_t    zfield_unpack( field_t* fp );

/*! @fn result_t    zfield_pack( field_t* fp );
 *  @brief Упаковка поля во фрейм
 *      Перед вызовов данной функции обязательно должен производится поиск поля с
 *  помощью функции zfield_find().
 *  @param fp - указатель на структуру, описывающую поле
 *  @return В случае успеха возвращается ENOERR, иначе значение <0.
 * */
result_t    zfield_pack( field_t* fp );

/*! @def GET_FIELD(field, fname )
 *  @brief  Макрос возвращает значение поля
 *  @param fp - указатель на структуру, описывающую поле
 *  @param fname - имя поля
 *  @return Возвращается указатель на поле в случае успеха, иначе 0
 * */
#define     GET_FIELD( fp, fname ) \
    ({ void     *fptr; \
       result_t res; \
        fptr = zfield_find( (fp), fname); \
        ASSERT1(0 == fptr,"Error: can not find the network field %d", fname ); \
        res = zfield_unpack( (fp) ); \
        ASSERT1( IS_ERROR(res), "Error: can not unpack the network field %d", fname); \
        IS_OK(res)?(fname##_TYPE *)fptr:(fname##_TYPE *)0; \
    })

/*! @def SET_FIELD(field, fname, value)
 *  @brief  Макрос устанавливает новое значение поля
 *  @param fp - указатель на структуру, описывающую поле
 *  @param fname - имя поля
 *  @param value - новое значение поля
 *  @return Возвращается указатель на поле в случае успеха, иначе 0
 * */
#define     SET_FIELD( fp, fname, value) \
    ({ void         *fptr; \
       result_t     res; \
        fptr = zfield_find( (fp), fname ); \
        ASSERT1(0 == fptr,"Error: can not find the network field %d", fname ); \
        *(fname##_TYPE *)fptr = (fname##_TYPE)value; \
        res = zfield_pack( (fp) ); \
        ASSERT1( IS_ERROR(res), "Error: can not pack the network field %d", fname); \
        IS_OK(res)?(fname##_TYPE *)fptr:(fname##_TYPE *)0; \
    })

/*!  @}  */

/*! @fn void    reorder( void *from, uint8_t  from_offset,
        void *to,  uint8_t to_offset, uint8_t len )
 *  @brief Копирование с переупорядочиванием байтов при необходимости, то есть
 *  если не совпадает сетевой порядок байт и на узле.
 *  @param from - откуда копировать
 *  @param from_offset - смещение относительно from
 *  @param to - куда копировать
 *  @param to_offset - смещение относительно назначения
 *  @param len - размер блока для копирования
 */
void    reorder( void *from, uint8_t  from_offset,void *to,  uint8_t to_offset, 
        uint8_t len );

/*! @def  OCTET( bcn_ptr, offset )  
 *  @brief  Получение доступа к октету с определённым смещением относительно указателя
 * */
#define     OCTET( ptr, offset )    (*((uint8_t *)ptr+offset))

#endif      /* _ZIGBEE_FIELD_H */


