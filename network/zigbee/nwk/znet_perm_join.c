//  vim:encoding=utf-8
/*! @file  znet_perm_join.c
 *  @brief Разрешение/запрещение присоединений
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1     
 *      Реализация примитивов NLME-PERMIT-JOINING.request ( см. 3.3.4.1 )
 *  и NLME-PERMIT-JOINING.confirm ( см. 3.3.4.2 ).      
 *  */

#include    <zcall.h>
#include    <znet_join.h>
#include    <zmac_attr.h>

/* Функция, вызываемая по окончании интервала времени, в течении которого было
 * разрешено присоединятся к данному узлу */
void    timer_fired( unidata_t  unidata )
{
    macAssociationPermit = FALSE;
}

/* Обработка запросов на разрешение (запрещение) просоединений к узлу */
void    net_permit_join( zcall_t    *zc )
{
    switch( ZARG(zc,zargs_pjoin_t,duration) ) {
        case    0x00: /* Запрет присоединений */
            macAssociationPermit = FALSE;
            break;
        case    0xff: /* Разрешение присоединений на неопределённое время */
            macAssociationPermit = TRUE;
            break;
        default: /* Разрешение присоединений в течении заданного промежутка времени */
            // timer_set( ZARG(zc, zargs_pjoin_t ,duration)*1000, 0 ); /* TODO */
            break;
    }
    ZARG(zc,zargs_pjoin_t,status) = SUCCESS;
    zcall_return( zc );
    return;
}

ZCALL_PROVIDE( ZCALL_NLME_PERMIT_JOIN, net_permit_join );

