//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_TX_H
#define     _ZIGBEE_MAC_TX_H
/*! @file  zmac_tx.h
 *  @brief Интерфейс передачи данных
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zneighbor.h>

/*! @defgroup ZIGBEE_MAC_TX  Интерфейс передачи данных
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 * */

/* Аргументы стековых вызовов передачи данных */
typedef struct {
    neighbor_t  *dst;           /*!< Вх. Указатель на узел назначения в таблице соседей. Или 0 для
                                 *   широковещательной передачи  */
    packet_t    *pkt;           /*!< Вх. Указатель на отправляемый пакет */
    unsigned    ack         :1; /*!< Вх. Необходимо ли полтверждение пакета: 1 - да, 0 - нет */    
    unsigned    cap         :1; /*!< Вх. Если 1 - то отправлять через CAP, иначе через GTS. */
    unsigned    indirect    :1; /*!< Вх. Если 1 - то непосредственная передача */
    unsigned                :5;
    uint8_t     handle;         /*!< Вх. Вых. Пользовательский описатель */
    zstatus_t   status;         /*!< Вых. Статус завершения процедуры отправки */
} zargs_mac_tx_t;
/*! @} */

/*! @defgroup ZIGBEE_MAC_TX_UNICAST  Интерфейс одноадресной передачи
 *  @ingroup  ZIGBEE_MAC_TX
 *  @{ 
 * */

/* Имя стекового вызова для одноадресной передачи */
#define     ZCALL_MAC_TX_UNICAST  109

/* Аргументы стекового вызова ZCALL_MAC_TX_UNICAST */
typedef     zargs_mac_tx_t  zargs_mac_utx_t;

/*! @} */

/*! @defgroup ZIGBEE_MAC_TX_BROADCAST  Интерфейс широковещательной передачи
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 * */

/* Имя стекового вызова для широковещательной передачи */
#define     ZCALL_MAC_TX_BROADCAST 110

/* Аргументы стекового вызова ZCALL_MAC_TX_BROADCAST. */
typedef     zargs_mac_tx_t  zargs_mac_btx_t;

/*! @} */

#endif      /* _ZIGBEE_MAC_TX_H */

