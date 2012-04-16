//  vim:encoding=utf-8
#ifndef     _ZIGBEE_CALL_H
#define     _ZIGBEE_CALL_H
/*! @file  zcall.h
 *  @brief Интерфейс стековых вызовов
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zigconf.h>

/*! @defgroup ZIGBEE_CALL Стековые вызовы
 *  @ingroup  ZIGBEE_CALL
 */

struct _zcall_t;
/*! Тип функции стекового вызова */
typedef     void  (* zcall_func_t)( struct _zcall_t* );

/* Структура, связывающая имя стекового вызова с первой функцией */
struct zcall_bind_t {
    uint16_t    name;
    zcall_func_t   func;
};

/* Структура данных стекового вызова */
typedef  struct _zcall_t {
    unsigned    busy        :1;     /*!<    Флаг использования стекового вызова */
    unsigned    reserved    :3;     /*!<    Используется реализацией    */
    unsigned    state       :4;     /*!<    Состояние вызова */
    unsigned                :8;
    const struct zcall_bind_t    *bind;   /*!<    Указатель на имя стекового вызова и начальную функцию */
    zcall_func_t    func;           /*!<    Текущая функция стековго вызова */
    zcall_func_t    callback;       /*!<    Функция обратного вызова */
    struct _zcall_t*    assoc_call; /*!<    Указатель на стековый вызов, связанный с данным */
    uint8_t     args[ Z_CALL_MAX_ARGS_SIZE ];   /*!< Аргументы стекового вызова */
} zcall_t;

/* Доступ к аргументу стекового вызова */
#define     ZARG( call, type, field ) (((type *)(call->args))->field)

/*! @def ZCALL_USES( name )
 *      Макрос определяет какие стековые вызовы используются
 *  @param name - имя стекового вызова
 * */
#define     ZCALL_USES( name ) _ZCALL_USES( name )
#define     _ZCALL_USES( name )     __ZCALL_USES( name )
#define     __ZCALL_USES( name )    \
    __attribute__((weak)) const struct zcall_bind_t __zcall_##name = { name, 0x0000 };

/* Инициализация стекового вызова */
#define     ZCALL_INIT( zcall_ptr, name, cb ) _ZCALL_INIT( zcall_ptr, name, cb )
#define     _ZCALL_INIT( zcall_ptr, name, cb ) __ZCALL_INIT( zcall_ptr, name, cb )
#define     __ZCALL_INIT( zcall_ptr, name, cb ) \
    { zcall_ptr->bind = &__zcall_##name; zcall_ptr->callback = cb; }

/* Возвратить имя стекового вызова */
#define     ZCALL_NAME(zcall_ptr) ((zcall_ptr)->bind->name)

/*! @defgroup ZIGBEE_CALL_EXT Внешний интерфейс стековых вызовов
 *  @ingroup  ZIGBEE_CALL_EXT
 *  @{ 
 *      Пользовательский интерфейс стековых вызовов.
 * */

/*! @fn zcall_t*    zcall_new();
 *  @brief Создание нового стекового вызова
 *  @return Возвращается указатель на стековый вызов в случае успеха, иначе 0.
 * */
zcall_t*    zcall_new();

/*! @fn result_t    zcall_invoke( zcall_t *zc );
 *  @brief Запуск стекового вызова
 *      Поле name при этом должно содержать имя вызова.
 *  Примечание: функция изменяет поле func!
 *  @param zc - указатель на стековый вызов
 *  @return В случае успеха возвращается ENOERR, иначе
 *      - ENOTFOUND - если по имени вызова не найдена стековая функция,
 *      - EINVAL - некорректное значение указателя на вызов, либо неверное значение 
 *          одного из полей стекового вызова,
 * */
result_t    zcall_invoke( zcall_t *zc );

/*! @fn     result_t    zcall_del( zcall_t *zc );
 *  @brief Удалить стековый вызов
 *  @param zc - указатель на стековый вызов
 *  @return В случае успеха возвращается ENOERR, иначе значение <0.
 * */
result_t    zcall_del( zcall_t *zc );

/*!  @}  */

/*! @defgroup ZIGBEE_CALL_INT Внутренний интерфейс стековых вызовов
 *  @ingroup  ZIGBEE_CALL_INT
 *  */

/*! @def ZCALL_PROVIDE( name, fn )
 *      Макрос определяет реализацию стекового вызова, которую предоставляет модуль.
 *  @param name - имя стекового вызова
 *  @param fn - функция, которая будет вызвана при zcall_invoke.
 * */
#define     ZCALL_PROVIDE( name, fn ) _ZCALL_PROVIDE( name, fn )
#define     _ZCALL_PROVIDE( name, fn )  __ZCALL_PROVIDE( name, fn )
#define     __ZCALL_PROVIDE( name, fn ) \
    const struct zcall_bind_t __zcall_##name = { name, fn };

/*! @fn result_t    zcall_return( zcall_t *zc );
 *  @brief Возврат из стекового вызова
 *  @param zc - указатель на стековый вызов
 *  @return В случае успеха возвращается ENOERR, иначе:
 *      - EINVAL - если одно из полей стекового вызова некорректно.
 * */
result_t    zcall_return( zcall_t *zc );

/*! @fn result_t    zcall_continue( zcall_t  *zc );
 *  @brief Продолжение выполнения стекового вызова
 *      Стековый вызов продолжает выполнятся с функции из поля func
 *  @param zc - указатель на стековый вызов
 *  @return  В случае успеха возвращается ENOERR, иначе:
 *      - EINVAL - если одно из полей стекового вызова некорректно.
 * */
result_t    zcall_continue( zcall_t  *zc );

/*!  @}  */

/*!  @}  */
#endif      /* _ZIGBEE_CALL_H */

