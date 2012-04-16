//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_CONST_H
#define     _ZIGBEE_NETWORK_CONST_H
/*! @file  znet_const.h
 *  @brief Константы сетевого уровня ZigBee
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <types.h>

/*! @defgroup ZIGBEE_NETWORK_CONST Константы сетевого уровня
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 *      Константы определяют свойства ( характеристики ) сетевого уровня.
 * */

/*! @def nwkcCoordinatorCapable
 *      Логический флаг, определяющий может ли устройство быть координатором.
 *  Значение 0 означает, что не может. Значение 1 означает, что устройство может 
 *  стать координатором.   
 * */
#ifndef nwkcCoordinatorCapable
#define     nwkcCoordinatorCapable  0x00
#endif

/*! @def nwkcDefaultSecurityLevel
 *      Используемый уровень безопасности по умолчанию.
 * */
#define nwkcDefaultSecurityLevel    0x00

/*! @def nwkcDiscoveryRetryLimit
 *    Максимальное число попыток осуществления поиска маршрута.     
 * */
#define nwkcDiscoveryRetryLimit     0x03

/*! @def nwkcMaxDepth
 *   Максимальное число хопов от координатора до узла.   
 * */
#define nwkcMaxDepth    0x0f

/*! @def nwkcMinHeaderOverhead
 *      Минимальный размер сетевого заголовока в октетах.   
 * */
#define nwkcMinHeaderOverhead   0x08

/*! @def nwkcProtocolVersion
 *      Версия протокола сетевого уровня ZigBee на данном устройстве.    
 * */
#define nwkcProtocolVersion     0x02

/*! @def nwkcWaitBeforeValidation
 *      Интервал времени в миллисекундах между получением
 *  ответа на запрос маршрута и отправкой сообщения для проверки этого
 *  маршрута.   
 * */
#define nwkcWaitBeforeValidation    0x500

/*! @def nwkcRepairThreshold
 *      Максимальное число коммуникационных ошибок, после которых
 *  запускается механизм востановления маршрута.   
 * */
#define nwkcRepairThreshold     0x03

/*! @def nwkcRouteDiscoveryTime
 *      Продолжительность поиска маршрута в миллисекундах.   
 * */
#define nwkcRouteDiscoveryTime  0x2710

/*! @def nwkcMaxBroadcastJitter
 *      Максимальное значение случайной задержки отправки широковещательного
 *  сообщения в миллисекундах.
 * */
#define nwkcMaxBroadcastJitter  0x40

/*! @def nwkcInitialRREQRetries
 *      Максимальное число попыток отправки запроса на поиск маршрута.
 * */
#define nwkcInitialRREQRetries  0x03

/*! @def nwkcRREQRetries
 *      Максимальное число попыток пересылки запроса на поиск маршрута
 *  промежуточными ZigBee-маршрутизаторами или ZigBee-координатором.
 * */
#define nwkcRREQRetries     0x02

/*! @def nwkcRREQRetryInterval
 *      Число миллисекунд между повторными попытками отправки запроса
 *  на поиск маршрута.   
 * */
#define nwkcRREQRetryInterval   0xfe

/*! @def nwkcMinRREQJitter
 *      Минимальное дрожание ( в 2-х секунднах слотах ) для передачи
 *  запроса на поиск маршрута на промежуточных узлах   
 * */
#define nwkcMinRREQJitter   0x01

/*! @def nwkcMaxRREQJitter
 *      Максимальное дрожание ( в 2-х секунднах слотах ) для передачи
 *  запроса на поиск маршрута на промежуточных узлах   
 * */
#define nwkcMaxRREQJitter   0x40

/*  @}  */
#endif  /* _ZIGBEE_NETWORK_CONST_H */

