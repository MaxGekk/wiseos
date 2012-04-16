//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_SYNC_H
#define     _ZIGBEE_MAC_SYNC_H
/*! @file  znet_mac.h
 *  @brief Синхронизация с координатором
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup   ZIGBEE_MAC_SYNC Синхронизация с координатором
 *  @ingroup    ZIGBEE_MAC
 *  @{ 
 * */

/* Имя стекового вызова для синхронизации */
#define     ZCALL_MLME_SYNC     114

/* Аргументы стекового вызова ZCALL_MLME_SYNC */
typedef struct {
    uint8_t     logical_channel;    /*!< Вх. Номер канала, на котором необходимо проводить синхронизацию */
    bool_t      track_beacon;       /*!< Вх. Поддерживать ли постоянную синхронизацию с кадрами-маяками координатора.
                                     *   Если поле равно TRUE, то да, иначе синхронизоваться только с одним кадром-маяком
                                     *   координатора */
} zargs_mlme_sync_t;

/* Имя стекового вызова, извещающего о потере синхронизации */
#define     ZCALL_MLME_SYNC_LOSS    115

/*! Причины потери синхронизации */
typedef enum {
    ZMAC_PAN_ID_CONFLICT = 0,
    ZMAC_REALIGNMENT,
    ZMAC_BEACON_LOST
} loss_reason_t;

/* Аргументы стекового вызова ZCALL_MLME_SYNC_LOSS */
typedef struct {
    loss_reason_t   loss_reason;    /*!< Вх. Причина потери синхронизации. Допустимы следующие значения аргумента:
                                     *      -   ZMAC_PAN_ID_CONFLICT
                                     *      -   ZMAC_REALIGNMENT
                                     *      -   ZMAC_BEACON_LOST
                                     * */
    uint8_t     logical_channel;    /*!< Вх. Номер канала, на котором была потеряна синхронизация */
    panid_t    panid;               /*!< Вх. PANID узла, с которым была потеряна синхронизация */
} zargs_mlme_sloss_t;

/*  @}  */
#endif  /*  _ZIGBEE_MAC_SYNC_H   */

