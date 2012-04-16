//  vim:encoding=utf-8
/*! @file  znet_sync.c
 *  @brief Реализация синхронизации с координатором
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1
 *
 *      Основная задача модуля - это опрос координатора на предмет наличия у него
 *  данных для нас, запрос и приём этих данных. Это нужно для непрямой передачи от
 *  родительского узла к его потомкам. При этом дочерние узлы могут переходить на 
 *  неопределённое время в состояние сна, а после просыпания запрашивать от координатора
 *  поступившие за это время для них данные. Модуль предоставляет реализацию примитивов
 *  NLME-SYNC.request ( см. раздел 3.3.10.1 ) и MLME-SYNC.confirm ( раздел 3.3.10.2 ). 
 *
 *  */

#include    <zcall.h>
#include    <zmac_attr.h>
#include    <zmac_sync.h>
#include    <zmac_poll.h>
#include    <znet_sync.h>
#include    <znet_relative.h>

ZCALL_USES( ZCALL_MLME_POLL );
ZCALL_USES( ZCALL_MLME_SYNC );
ZCALL_USES( ZCALL_NLME_SYNC_LOSS );

/* Синхронизация на канальном уровне завершена */
void    net_sync_done( zcall_t   *mac_call )
{
    zcall_t *net_call = mac_call->assoc_call;
    ZARG( net_call, zargs_nlme_sync_t, status ) = ZARG(mac_call, zargs_mlme_poll_t, status );
    zcall_del( mac_call );
    /* Возвращаем результат синхронизации вышележащему уровню */
    zcall_return( net_call );
    return;
}

/* Обработка запроса на синхронизацию */
void    net_sync( zcall_t   *zc )
{
    neighbor_t  *parent_ptr = get_parent();
    zcall_t     *mac_call   = zcall_new();
    /* Создаём новый стековый вызов для канального уровня и ищем в таблице соседей запись 
     * родительского узла, которого мы будем опрашивать */
    if( IN_TABLE(parent_ptr) && ( mac_call != (zcall_t *)0 ) ) {
        mac_call->assoc_call = zc;  /* связываем канальный вызов и сетевой*/
        /* Синхронизация по разному работает в режиме с кадрами-маяками и без кадров-маяков.
         * В режиме без кадров-маяков мы просим канальный уровень немедленно опросить нашего
         * координатора, а в режиме с кадрами-маяками устанавливаем атрибут macAutoRequest в TRUE
         * и канальный уровень сам при приёме очередного кадра-маяка от координатора определяет есть
         * ли для нас данные и запрашивает их. */
        if( macBeaconOrder == 15 ) {
            /* Режим без периодического испускания кадров-маяков */
            ASSERT( ZARG(zc, zargs_nlme_sync_t, track) == TRUE,"Warning: invalid argument 'track'" );
            /* Вызываем примитив канального уровня MLME-POLL.request */
            ZCALL_INIT( mac_call, ZCALL_MLME_POLL, net_sync_done );
            ZARG( mac_call, zargs_mlme_poll_t, coord_ptr ) = parent_ptr;
            if( IS_OK(zcall_invoke( mac_call )) )
                return;
        } else {
            /* Режим с периодическими кадрами-маяками */
            macAutoRequest = TRUE;  /* Флаг автоматического запроса данных */
            /* Вызываем примитив канального уровня MLME-SYNC.request */
            ZCALL_INIT( mac_call, ZCALL_MLME_SYNC,0 );
            ZARG(mac_call, zargs_mlme_sync_t, track_beacon) = ZARG(zc, zargs_nlme_sync_t, track);
            ZARG(mac_call, zargs_mlme_sync_t, logical_channel) = parent_ptr->channel;
            if( IS_OK(zcall_invoke( mac_call )) ) {
                /* Для MLME-SYNC.request нет .confirm, поэтому сразу выходим. Вызов mac_call 
                 * автоматически удалиться системой, так как callback == 0. */
                ZARG(zc, zargs_nlme_sync_t,status) = SUCCESS;
                zcall_return( zc );
                return;
            }
       } 
       zcall_del( mac_call );
    }
    ZARG(zc, zargs_nlme_sync_t,status) = NWK_INVALID_REQUEST;
    zcall_return( zc );
    return;
}
ZCALL_PROVIDE( ZCALL_NLME_SYNC, net_sync );

/* Обработка потери синхронизации с координатором */
void    net_sync_loss( zcall_t  *zc )
{
    /* Уведомляем вышележащий уровень, переиспользуя канальный вызов */
    ZCALL_INIT( zc, ZCALL_NLME_SYNC_LOSS, 0 );
    zcall_invoke( zc );
    return;
}
ZCALL_PROVIDE( ZCALL_MLME_SYNC_LOSS, net_sync_loss );

