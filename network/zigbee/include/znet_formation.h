//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_FORMATION_H
#define     _ZIGBEE_NETWORK_FORMATION_H
/*! @file  znet_formation.h
 *  @brief Интерфейсы формирования сети
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup ZIGBEE_NETWORK_FORMATION Формирование сети ZigBee-координатором
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 *  */

/*! @def  ZCALL_NET_FORMATION   
 *  @brief Имя стекового вызова формирования сети ZigBee-координатором 
 *  */
#define     ZCALL_NET_FORMATION  6

/*! @typedef  zargs_nform_t 
 *  @brief Аргументы стекового вызова ZCALL_NET_FORMATION 
 *  */
typedef     struct {
    uint32_t    channels;       /*!< Вх. Битовая маска каналов для сканирования 
                                 *   Если n-ый бит установлен в 1, то сканировать, иначе нет */
    uint8_t     duration;       /*!< Вх. Продолжительность сканирования. Сканировать в течении 
                                 *   aBaseSuperframeDuration*(2^duration+1) символов */
    uint8_t     beacon_order;   /*!< Вх. Аргумент, определяющий период испускания кадров-маяков.
                                 *   Период равен aBaseSuperframeDuration*2^beacon_order символов, если 0<=bo<15 
                                 *   */
    uint8_t     sf_order;       /*!< Вх. Аргумент, определяющий длину активной части суперфрейма ( для bo<15).
                                 *   Продолжительность активной части суперфрейма равна: aBaseSuperframeDuration*2^sf_order */
    bool_t      batt_life_ext;  /*!< Вх. Если TRUE - включить расширенный режим работы от батарей питания */
    zstatus_t   status;         /*!< Вых. Статус завершения процедуры формирования сети */
} zargs_nform_t;

/*! @}  */
#endif  /*  _ZIGBEE_NETWORK_FORMATION_H   */

