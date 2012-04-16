//  vim:encoding=utf-8
/*! @file  zcall.c
 *  @brief Реализация стековых вызовов
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <task.h>
#include    <mem.h>
#include    <debug.h>
#include    <zcall.h>
#include    <znet_reset.h>

/* Пул стековых вызовов */
zcall_t     zcalls[ Z_CALL_POOL_SIZE ];     

/* Макрос возвращает состояние (внутреннее) стекового вызова */
#define SYS_STATE( zcall_ptr ) ((zcall_ptr)->reserved)
#define SYS_STATE_DEL           0x00    /* Стековый вызов удалён */
#define SYS_STATE_NEW           0x01    /* Стековый вызов создан */
#define SYS_STATE_PRE_CALL      0x02    /* Перед вызовом очередной функции */
#define SYS_STATE_PRE_CALLBACK  0x03    /* Перед вызовом callback*/
#define SYS_STATE_CALL          0x04    /* Выполнение стекового вызова в функции */
#define SYS_STATE_CALLBACK      0x05    /* Выполнение callback */

/* Проверка корректности указателя на стековый вызов  */
#define CHECK_PTR( zcall_ptr ) \
    /* Проверка того, что указатель на пул стековых вызовов */ \
    ASSERT1( ( zcall_ptr < zcalls )||( &zcalls[Z_CALL_POOL_SIZE] <= zcall_ptr ), \
            "Invalid zcall_ptr [%x] : it is out of stack call pool", (uint16_t)zcall_ptr );\
    /* Проверка того, что указатель на один из вызовов в пуле */ \
    ASSERT1( (zcall_ptr - zcalls) % sizeof(zcall_t), \
            "Invalid zcall_ptr [%x] : it is not point to stack call", (uint16_t)zcall_ptr );

/*  Создание нового стекового вызова */
zcall_t*    zcall_new()
{
    uint16_t i;
    /* Поиск свободного места в пуле */
    for( i=0; i<Z_CALL_POOL_SIZE; i++ )
        if( 0 == zcalls[i].busy )
            break;

    /* Если пул полон, то возвращаем 0*/
    if( Z_CALL_POOL_SIZE <= i )
        return (zcall_t *)0;

    /* Инициализируем поля стекового вызова */
    zcalls[i].busy = 1;
    SYS_STATE( &zcalls[i] ) = SYS_STATE_NEW; /* Внутреннее состояние вызова */
    zcalls[i].state = 0;    /* Пользовательское состояние вызова по умолчанию */
    zcalls[i].bind = (struct zcall_bind_t *)0;
    zcalls[i].func = zcalls[i].callback = (zcall_func_t)0;
    zcalls[i].assoc_call = (zcall_t *)0;
    return &zcalls[i];
}

/* Удаление стекового вызова */
result_t    zcall_del( zcall_t *zc )
{
    CHECK_PTR( zc );
    SYS_STATE(zc) = SYS_STATE_DEL;
    zc->busy = 0;
    return ENOERR;
}

/* Системная задача, в которой осуществляется вызов функци */
void _zcall()
{
    uint16_t i;
    /* Бежим по пулу и ищем вызовы требующие выполнения. 
     * Явных приоритетов у вызовов нет, но, фактически, чем ближе вызов к началу
     * пула тем выше его приоритет */
    for( i=0; i<Z_CALL_POOL_SIZE; i++ ) {
        if( 0 == zcalls[i].busy )
            continue;
        switch( SYS_STATE( &zcalls[i] ) ) {
            case SYS_STATE_PRE_CALL: /* Продолжение обработки вызова */
                SYS_STATE( &zcalls[i] ) = SYS_STATE_CALL;
                (* zcalls[i].func)( &zcalls[i] );
                break;
            case SYS_STATE_PRE_CALLBACK: /* Возврат из вызова */
                SYS_STATE( &zcalls[i] ) = SYS_STATE_CALLBACK;
                (* zcalls[i].callback)( &zcalls[i] );
                break;
            default: break;
        }
    }
}
TASK( _zcall, 0 ); /* Обработка вызовов производится в задачах */

/* Начало обработки вызова. Вызов первой функции. */
result_t    zcall_invoke( zcall_t *zc )
{
    CHECK_PTR( zc );
    ASSERT( 0 == zc->busy, "Error: zcall is not allocated");
    ASSERT( 0 == zc->bind, "Error: zcall is not binded");

    zc->func = zc->bind->func;

    return zcall_continue( zc );
}

/* Возврат из вызова */
result_t    zcall_return( zcall_t *zc )
{
    CHECK_PTR( zc );
    ASSERT( 0 == zc->busy, "Error: zcall is not allocated");

    /* Так как функция callback не задана, то вызывающая сторона никак 
     * не сможет определить когда нужно удалить стековый вызов. Поэтому его
     * нужно удалить именно здесь */
    if( (zcall_func_t)0 == zc->callback ) {
        zc->busy = 0;
        return ENOERR;
    }

    SYS_STATE( zc ) = SYS_STATE_PRE_CALLBACK;
    POST_TASK( _zcall );

    return ENOERR;
}

/* Продолжение выполнения вызова */
result_t    zcall_continue( zcall_t  *zc )
{
    CHECK_PTR( zc );
    ASSERT( 0 == zc->busy, "Error: zcall is not allocated");

    if( (zcall_func_t)0 == zc->func )
        return EINVAL;

    /* Вызов функции в задаче */
    SYS_STATE(zc) = SYS_STATE_PRE_CALL;
    POST_TASK( _zcall );

    return ENOERR;
}

/* Сброс пула вызовов */
void    reset_zcall()
{
    mset(zcalls, Z_CALL_POOL_SIZE*sizeof(zcall_t),0);
}

REG_RESET( reset_zcall );

