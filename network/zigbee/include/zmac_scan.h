//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_SCAN_H
#define     _ZIGBEE_MAC_SCAN_H
/*! @file  zmac_scan.h
 *  @brief Интерфейс сканирования каналов
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */
#include    <ztypes.h>

/*! @defgroup ZIGBEE_MAC_SCAN_ED Определение уровня энергии на каналах 
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 *  */

/*! @def  ZCALL_MAC_SCAN_ED   
 *  @brief Имя стекового вызова для определения уровня энергии на каналах
 *  */
#define     ZCALL_MAC_SCAN_ED   103

typedef     struct {
    uint32_t    channels;      /*!< Вх. Битовая маская каналов подлежащих сканированию. 
                                * 27 бит (b0,b1,...,b26) соответсвуют каналам. Если соответсвующий
                                * бит установлен в 1, то канал должен быть просканирован, иначе 0 
                                *   Вых. Битовая маска каналов с уровнем энергии меньшим чем max_energy */
    uint8_t     duration;      /*!< Вх. Аргумент, определяющий продолжительность сканирования каждого
                                *   канала. Продолжительность aBaseSuperframeOrder*(2^duration+1) */
    uint8_t     max_energy;    /*!< Вх. Максимально допустимый уровень энергии на канале */
    zstatus_t   status;        /*!< Вых. Статус завершения процедуры сканирования */
} zargs_escan_t;
/*! @}  */

/*! @defgroup ZIGBEE_MAC_SCAN_ACTIVE Активное сканирование
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 *      Данный вызов только инициирует процедуру сканирования. Информация о принятых
 *  кадрах маяках возвращается стековым вызовом ZCALL_MAC_BEACON_INDICATION.
 *  */

/*! @def  ZCALL_MAC_SCAN_ACTIVE   
 *  @brief Имя стекового вызова для активного сканирования
 *  */
#define     ZCALL_MAC_SCAN_ACTIVE  104

/*! @typedef    zargs_ascan_t
 *  @brief  Аргументы вызова ZCALL_MAC_SCAN_ACTIVE
 * */
typedef     struct {
    uint32_t    channels;      /*!< Вх. Битовая маская каналов подлежащих сканированию. 
                                * 27 бит (b0,b1,...,b26) соответсвуют каналам. Если соответсвующий
                                * бит установлен в 1, то канал должен быть просканирован, иначе 0 
                                *   Вых. Битовая маска просканированных каналов */
    uint8_t     duration;      /*!< Вх. Аргумент, определяющий продолжительность сканирования каждого
                                *   канала. Продолжительность aBaseSuperframeOrder*(2^duration+1) */
    zstatus_t   status;        /*!< Вых. Статус завершения процедуры сканирования */
} zargs_ascan_t;
/*! @}  */

/*! @defgroup ZIGBEE_MAC_SCAN_ORPHAN Сканирование в поиске потерянного родителя
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 *  */

/*! @def  ZCALL_MAC_SCAN_ORPHAN   
 *  @brief Имя стекового вызова для поиска родителя
 *  */
#define     ZCALL_MAC_SCAN_ORPHAN  105

/*! @typedef    zargs_oscan_t
 *  @brief  Аргументы вызова ZCALL_MAC_SCAN_ORPHAN
 * */
typedef     struct {
    uint32_t    channels;      /*!< Вх. Битовая маская каналов подлежащих сканированию. 
                                * 27 бит (b0,b1,...,b26) соответсвуют каналам. Если соответсвующий
                                * бит установлен в 1, то канал должен быть просканирован, иначе 0 
                                *   Вых. Битовая маска просканированных каналов */
    uint8_t     duration;      /*!< Вх. Аргумент, определяющий продолжительность сканирования каждого
                                *   канала. Продолжительность aBaseSuperframeOrder*(2^duration+1) */
    zstatus_t   status;        /*!< Вых. Статус завершения процедуры сканирования */
} zargs_oscan_t;
/*! @}  */

/*! @defgroup ZIGBEE_MAC_SCAN_PASSIVE Пассивное сканирование
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 *      Данный вызов только инициирует процедуру сканирования. Информация о принятых
 *  кадрах маяках возвращается стековым вызовом ZCALL_MAC_BEACON_INDICATION.
 *  */

/*! @def  ZCALL_MAC_SCAN_PASSIVE
 *  @brief Имя стекового вызова для пассивного сканирования
 *  */
#define     ZCALL_MAC_SCAN_PASSIVE  106

/*! @typedef    zargs_pscan_t
 *  @brief  Аргументы вызова ZCALL_MAC_SCAN_PASSIVE
 * */
typedef     zargs_ascan_t   zargs_pscan_t;

/*! @}  */

#endif  /*  _ZIGBEE_MAC_SCAN_H   */

