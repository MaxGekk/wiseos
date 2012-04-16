//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_START_H
#define     _ZIGBEE_MAC_START_H
/*! @file  zmac_start.h
 *  @brief Интерфейс запуска подсети
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup ZIGBEE_MAC_START  Запуск (под)сети
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 *  */

/*! @def  ZCALL_MAC_START   
 *  @brief Имя стекового вызова для запуска (под)сети
 *  */
#define     ZCALL_MAC_START  108

/*! @typedef  zargs_nstart_t 
 *  @brief Аргументы стекового вызова ZCALL_MAC_START 
 *  */
typedef     struct {
    s_panid_t   panid;              /*!<    Вх. Идентификатор сети, который должен использоваться устройством */
    unsigned    channel         :5; /*!<    Вх. Номер канала */
    unsigned    pan_coord       :1; /*!<    Вх. Если 1, то устройство хочет стать PAN координатором. */
    unsigned    batt_life_ext   :1; /*!<    Вх. Использовать ли расширенный режим работы от батареи */
    unsigned    realignment     :1; /*!<    Вх. Отправлять ли извещение подчинённым узлам об изменении параметров суперфрейма */
    unsigned    beacon_order    :4; /*!<    Вх. Аргумент, определяющий период испускания кадров-маяков.
                                     *      Период равен aBaseSuperframeDuration*(2^beacon_order) символов, если 0<=bo<15 
                                     *   */
    unsigned    sf_order        :4; /*!<    Вх. Аргумент, определяющий длину активной части суперфрейма.
                                     *      Период равен aBaseSuperframeDuration*(2^sf_order) символов, если 0<=bo<15 
                                     *   */
    uint32_t    start_time;         /*!<    Вх. Время отправки первого кадра-маяка в символах */
    zstatus_t   status;             /*!<    Вых. Статус завершения вызова. */
} zargs_mstart_t;

/*! @}  */
#endif  /*  _ZIGBEE_MAC_START_H   */

