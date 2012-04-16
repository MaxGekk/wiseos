//  vim:encoding=utf-8
/*! @file  znet_orphan_child.c
 *  @brief Присоединение дочернего узла к потеряному родителю
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1  
 *
 *      Модуль реализует механизм поиска и востановления связи с 
 *  потерянным родительским узлом. См. описание примитивов NLME-JOIN.request
 *  ( раздел 3.3.6.1 ) и NLME-JOIN.confirm ( раздел 3.3.6.3 ), а также
 *  функциональное описание из раздела 3.7.1.3.3.2.         
 *
 *  */

#include    <ztypes.h>
#include    <debug.h>
#include    <zcall.h>
#include    <znet_attr.h>
#include    <znet_join.h>
#include    <zmac_scan.h>

ZCALL_USES( ZCALL_MAC_SCAN_ORPHAN );

/* Окончание процедуры orphan-сканирования */
void    znet_oscan_done( zcall_t *mc )
{
    zcall_t     *zc = mc->assoc_call;

    ASSERT( (zcall_t *)0 == zc, "Error: this orphan scan call isn't associated with a join call" );
    /* В случае если родитель не найден возвращаем NWK_NO_NETWORKS.
     * Определить найден родитель или нет можно только по статусу 
     * orphan-сканирования.
     * */
    if( ZARG( mc, zargs_oscan_t, status ) != SUCCESS )
        ZARG( zc, zargs_ojoin_t, status ) = NWK_NO_NETWORKS;
    zcall_del( mc );
    zcall_return( zc );
    return;
}

/* Обработка запроса на востановление связи с родительским узлом */
void    znet_orphan_child( zcall_t  *zc )
{
    zcall_t     *mc;
    zargs_oscan_t   *marg = (zargs_oscan_t *)(mc->args);
    zargs_ojoin_t   *narg = (zargs_ojoin_t *)(zc->args);

    /* Если узел не присоединён к сети, то востанавливать нечего. 
     * Пояснения в разделе 3.7.1.3.3.2 */
    if( nwkExtraAttr.is_joined == FALSE )
        goto abort_orphan_child;

    mc = zcall_new();
    if( (zcall_t *)0 == mc )
        goto abort_orphan_child;

    mc->assoc_call = zc;
    marg->channels = narg->channels;
    marg->duration = narg->duration;

    ZCALL_INIT( mc, ZCALL_MAC_SCAN_ORPHAN, znet_oscan_done );
    if( IS_OK(zcall_invoke(mc)) )
        return;

    zcall_del( mc );
abort_orphan_child:
    narg->status= NWK_INVALID_REQUEST;
    zcall_return( zc );
    return;
}

ZCALL_PROVIDE( ZCALL_NLME_OJOIN, znet_orphan_child );

