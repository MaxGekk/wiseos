//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_BNOTIFY_H
#define     _ZIGBEE_MAC_BNOTIFY_H
/*! @file  zmac_bnotify.h
 *  @brief Интерфейс извещения о приёме кадра-маяка
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zpacket.h>

/*! @defgroup ZIGBEE_MAC_BEACON_NOTIFY  Извещение о приёме кадра-маяка
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 *  */

/*! @def  ZCALL_MAC_BEACON_NOTIFY   
 *  @brief Имя стекового вызова извещающего о приёме кадра-маяка
 *  */
#define     ZCALL_MAC_BEACON_NOTIFY   107

/*! @typedef  zargs_mbnotify_t 
 *  @brief Аргументы стекового вызова ZCALL_MAC_BEACON_NOTIFY
 *  */
typedef     struct {
    uint8_t     channel;    /*!< Номер канала, на котором был принят кадр-маяк */
    uint8_t     bsn;        /*!< Последовательный номер кадра-маяка */
    sf_spec_t   sf_spec;    /*!< Спецификатор суперфрейма */
    packet_t    *bcn_payload;   /*!< Указатель на пакет с полезной нагрузкой */
    s_panid_t   panid;      /*!< Идентификатор сети узла, от которого был получен кадр-маяк */
    union {
        s_addr_t    saddr;  /*!< Короткий адрес источника кадра-маяка */
        e_addr_t    eaddr;  /*!< Длинный адрес источника кадра-маяка */
    };
    addr_mode_t     addr_mode;  /*!< Используемый в кадре-маяке режим адресации */
} zargs_mbnotify_t;

/*! @}  */
#endif  /*  _ZIGBEE_MAC_START_H   */

