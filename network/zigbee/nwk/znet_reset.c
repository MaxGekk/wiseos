//  vim:encoding=utf-8
/*! @file  znet_reset.c
 *  @brief Сброс сетевого уровня ZigBee
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1     
 *      Модуль отвечает за сброс ( переинициализацию ) сетевого и
 *  нижележащего канального уровня. Реализуются примитивы запроса
 *  сброса NLME-RESET.request ( 3.3.9.1.1 ) и подтверждения сброса
 *  NLME-RESET.confirm ( 3.3.9.2.1 ).      
 *  */

#include    <debug.h>
#include    <ztypes.h>
#include    <zcall.h>
#include    <zmac_reset.h>
#include    <znet_reset.h>

ZCALL_USES( ZCALL_MLME_RESET );

/* Адреса функций сброса модулей сетевого уровня расположены непрерывно
 * в определённой области памяти. _reset_begin - первая функция в этой области,
 * a _reset_end - фиктивная функция, адрес которой расположен сразу за областью
 * с адресами функций инициализации. */
extern net_reset_ft _reset_begin;
extern net_reset_ft _reset_end;

/* Обработка уведомления об окончании процедуры сброса канального уровня */
void    net_reset_done( zcall_t  *zc )
{
    zstatus_t   status;
    zcall_t *net_reset_call;

    ASSERT( zc->bind->name != ZCALL_MLME_RESET, "Incorrect zcall name" );
    ASSERT( 0 == zc->assoc_func, "assoc_func == NULL" );

    net_reset_call = zc->assoc_call;
    status = ZARG(zc, zargs_mlme_reset_t, status );
    ZARG( net_reset_call, zargs_nlme_reset_t, status ) = status;
    zcall_del(zc);

    if( SUCCESS == status ) {
        /* Резетим сетевой уровень*/
        net_reset_ft *nr = &_reset_begin;
        for(; nr < &_reset_end; nr++ )
            (*nr)();
    }

    zcall_return( net_reset_call );
    return;
}

/* Переинициализация сетевого уровня */
void    net_reset( zcall_t  *zc )
{
    __label__ error;
    /* Сброс сетевого уровня начинаем со сброса канального уровня */
    zcall_t *mac_reset_call;
    if( 0 == ( mac_reset_call = zcall_new() ) )
        goto error;

    ZCALL_INIT( mac_reset_call, ZCALL_MLME_RESET, net_reset_done );
    mac_reset_call->assoc_call = zc;
    /* Принудительно сбрасываем атрибуты канального уровня (см. 3.3.9.1.3 ) */
    ZARG( mac_reset_call, zargs_mlme_reset_t, set_default_pib ) = TRUE;

    if( IS_OK( zcall_invoke( mac_reset_call ) ))
        return;

    zcall_del( mac_reset_call );

error:
    ((zargs_nlme_reset_t *)(zc->args))->status = MAC_DISABLE_TRX_FAILURE;
    zcall_return(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_NLME_RESET, net_reset );

