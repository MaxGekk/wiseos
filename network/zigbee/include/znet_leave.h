//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_LEAVE_H
#define     _ZIGBEE_NETWORK_LEAVE_H
/*! @file  znet_leave.h
 *  @brief Выход из сети
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zmac_rx.h>

/*! @defgroup ZNWK_LEAVE_SELF_RFD Самостоятельный выход из сети конечного устройства
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 * */

/* Имя стекового вызова для самостоятельного выхода из сети конечного устройства */
#define     ZCALL_LEAVE_SELF_RFD     7

/* Аргументы стекового вызова ZCALL_LEAVE_SELF_RFD */
typedef struct {
    zstatus_t   status;     /*!< Вых. В случае успеха возвращается SUCCESS, иначе
                             *      - NWK_INVALID_REQUEST - вызов не может быть выполнен в текущем состоянии
                             *          устройства. Вызов может быть выполнен только для конечных устройств. */
} zargs_srleave_t;
/*! @} */

/*! @defgroup ZNWK_LEAVE_SELF_FFD Самостоятельный выход из сети ZigBee-координатора или маршрутизатора
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 * */

/* Имя стекового вызова для самостоятельного выхода из сети полнофункционального узла */
#define     ZCALL_LEAVE_SELF_FFD     8

/* Аргументы стекового вызова ZCALL_LEAVE_SELF_FFD */
typedef struct {
    unsigned    remove_children     :1;    /*!< Вх. Команда на явное удаление дочерних узлов */
    unsigned    rejoin              :1;    /*!< Вх. Команда на переподсоединение дочерних узлов */
    unsigned                        :6;
    zstatus_t   status;             /*!< Вых. В случае успеха возвращается SUCCESS, иначе
                                     *      - NWK_INVALID_REQUEST - вызов не может быть выполнен в текущем состоянии
                                     *          устройства. Вызов может быть выполнен только для полнофункциональных устройств. */
} zargs_sfleave_t;
/*! @} */

/*! @defgroup ZNWK_LEAVE_SELF_INDICATION Извещение о принудительном выходе из сети
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 * */

#define     ZCALL_LEAVE_SELF_INDICATION  9

/* Аргументы стекового вызова ZCALL_LEAVE_SELF_INDICATION */
typedef     struct {
    bool_t  rejoin;     /*!< Если TRUE - то узлу рекомендовано переподключиться */
} zargs_sileave_t;

/*! @} */


/*! @defgroup ZNWK_LEAVE Удаление дочернего узла из сети
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 * */

/* Имя стекового вызова для удаления дочернего узла из сети */
#define     ZCALL_LEAVE    10

/* Аргументы стекового вызова ZCALL_LEAVE */
typedef struct {
    e_addr_t    dev_addr;           /*!< Вх. Длинный адрес удаляемого дочернего узла */
    unsigned    remove_children :1; /*!< Вх. Команда на явное удаление дочерних узлов */
    unsigned    rejoin          :1; /*!< Вх. Команда на переподсоединение дочерних узлов */
    unsigned    reuse_addr      :1; /*!< Вх. Может ли быть переиспользованным адрес дочернего узла. TRUE - если да.*/
    unsigned    silent          :1; /*!< Вх. Если TRUE - то не отправлять дочернему узлу никаких пакетов */
    unsigned                    :4;
    zstatus_t   status;             /*!< Вых. В случае успеха возвращается SUCCESS, иначе
                                     *      - NWK_INVALID_REQUEST - вызов не может быть выполнен в текущем состоянии
                                     *          устройства.*/
} zargs_leave_t;
/*! @} */

/*! @defgroup ZNWK_LEAVE_INDICATION Извещение о выходе из сети дочернего узла
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 * */

#define     ZCALL_LEAVE_INDICATION  11

/* Аргументы стекового вызова ZCALL_LEAVE_INDICATION */
typedef     struct {
    e_addr_t    dev_addr;   /*!< Длинный адрес узла, вышедшего из сети */
} zargs_ileave_t;

/*! @} */

/* Имя стекового вызова для приёма команд о выходе из сети */
#define     ZCALL_NET_LEAVE_RX    12

/* Аргументы стекового вызова ZCALL_NET_LEAVE_RX */
typedef     zargs_mdata_rx_t    zargs_leave_cmd_t;

#define     LEAVE_REJOIN    (1<<5)
#define     LEAVE_REQUEST   (1<<6)
#define     LEAVE_RMCHLD    (1<<7)

#endif      /* _ZIGBEE_NETWORK_LEAVE_H */

