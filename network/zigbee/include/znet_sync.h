//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_SYNC_H
#define     _ZIGBEE_NETWORK_SYNC_H
/*! @file  znet_sync.h
 *  @brief Синхронизация с координатором
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup   ZIGBEE_NETWORK_SYNC Синхронизация с координатором
 *  @ingroup    ZIGBEE_NETWORK
 *  @{ 
 * */

/* Имя стекового вызова для синхронизации */
#define     ZCALL_NLME_SYNC     16

/* Аргументы стекового вызова ZCALL_NLME_SYNC */
typedef struct {
    bool_t  track;          /*!< Вх. Поддерживать ли постоянную синхронизацию с кадрами-маяками координатора.
                             *   Если поле равно TRUE, то да.*/
    zstatus_t   status;     /*!< Вых. Статус завершения процедуры синхронизации.
                             *   Допустимы следующие значения:
                             *      - SUCCESS - процедура синхронизации закончилась успешно,
                             *      - NWK_SYNC_FAILURE - не удалось синхронизироваться,
                             * */
} zargs_nlme_sync_t;

/* Имя стекового вызова, извещающего о потере синхронизации */
#define     ZCALL_NLME_SYNC_LOSS    17

/*  @}  */
#endif  /*  _ZIGBEE_NETWORK_SYNC_H   */

