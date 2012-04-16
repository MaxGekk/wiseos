#ifndef   _TYPES_H
#define   _TYPES_H

/*! @file types.h
 *  @brief Базовые типы данных системы.
 *  @author Max Gekk
 *  @author Igor Pankratov
 *  @date   декабрь 2006
 *  @version 0.1

 *  @defgroup types Базовые типы данных
 *  @ingroup kernel
 *  @{
 * */

typedef enum {
    FALSE   = 0,
    TRUE    = 1
} bool_t;

typedef unsigned char uint8_t;  /*!< Неотрицательное целое из диапазона 0..255 */
typedef unsigned short int uint16_t;    /*!< Неотрицательное целое из диапазона 0..65535 */
typedef unsigned long uint32_t; /*!< Неотрицательное целое из диапазона 0..4294967295 */
typedef unsigned long long uint64_t;    /*!< Неотрицательное целое из диапазона 0..18446744073709551615 */

typedef signed char int8_t;     /*!< Целое число из диапазона -127..127 */
typedef signed short int int16_t;   /*!< Целое число из диапазона -32767..32767 */
typedef signed long int32_t;    /*!< Целое число из диапазона -2147483647..2147483647 */
typedef signed long long int64_t;   /*!< Целое число из диапазона -9223372036854775807..9223372036854775807 */

typedef uint16_t    size_t;     /*!< Тип размеров */
typedef void*   unidata_t;

/*! @typedef result_t
 *  @brief Тип результата выполнения функции
 * */
typedef enum {
    ENOERR = 0,                 /*!< нет ошибки */
    ENOSYS = -1,                /*!< функция не реализована в системе */
    ENOMEM = -2,                /*!< недостаточно памяти */
    EINVAL = -3,                /*!< ошибочный аргумент */
    EBUSY = -4,                 /*!< система занята */
    ENOTFOUND = -5              /*!< ничего не найдено */
} result_t;

/*! @def IS_ERROR(res)
 *  @brief Является ли возвращённое значение ошибкой или нет.
 *  @param res результат выполнения функции
 *  @return 
 *      - 0<  произошла ошибка
 *      - 0   ошибки нет
 * */
#define     IS_ERROR( res )   ( (res) < 0 )

/*! @def IS_OK(res)
 *  @brief Является ли резльтат успешным или нет.
 *  @param res результат выполнения функции
 *  @return
 *      - 0<  результат был успешным
 *      - 0   ошибочный результат
 * */
#define     IS_OK( res )    ( 0 <= (res) )

typedef uint16_t    faddr_t;
#define EXISTS( func )  ( func != ((faddr_t)-1) )

/*! @} */
#endif                          /* _TYPES_H */
