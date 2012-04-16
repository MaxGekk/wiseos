//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_ATTR_H
#define     _ZIGBEE_NETWORK_ATTR_H
/*! @file  znet_attr.h
 *  @brief Атрибуты сетевого уровня 
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <types.h>
#include    <zigconf.h>
#include    <ztypes.h>
#include    <zneighbor.h>

/*! @defgroup  ZIGBEE_NETWORK_ATTR Атрибуты сетевого уровня
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 *      Атрибуты сетевого уровня ZigBee определяют внутреннее состояние
 *  уровня, а также позволяют влиять на работу последнего.
 * */

/*! @var nwkSequenceNumber
 *  Последовательный номер, используемый для идентификации исходящих сообщений.
 *  По умолчанию атрибут имеет случайное значение из диапазона от 0 до 255.
 * */
extern uint8_t  nwkSequenceNumber;

/*! @var nwkPassiveAckTimeout
 *      Максимальный интервал времени в секундах, в течении которого
 *  узел отслеживает отправку широковещательного сообщения родительским
 *  и дочерними узлами.
 * */
extern uint8_t  nwkPassiveAckTimeout;

/*! @var nwkMaxBroadcastRetries    
 *      Максимальное число повторных попыток передачи широковещательного
 *  пакета после первой неудачной. Допустимый диапазон от 0 до 5.
 * */
extern uint8_t  nwkMaxBroadcastRetries;

/*! @var nwkMaxChildren
 *      Максимальное число дочерних узлов.
 * */
extern uint8_t  nwkMaxChildren;

/*! @var nwkMaxDepth
 *      Максимальная возможная глубина устройства ( максимальный радиус сети ).
 *  Диапазон допустимых значений от 0x01 до nwkcMaxDepth.
 * */
extern uint8_t  nwkMaxDepth;

/*! @var nwkMaxRouters
 *      Максимльаное число дочерних роутеров у одного узла. Это число определяется
 * ZigBee-координатором.  Диапазон допустимых значений от 0x01 до 0xff.
 * */
extern uint8_t  nwkMaxRouters;

/*! @var nwkNeighborTable
 *      Таблица соседних узлов.
 * */
extern neighbor_t  nwkNeighborTable[ Z_NEIGHBOR_TABLE_SIZE ];

/*! @var nwkNetworkBroadcastDeliveryTime
 *      Интервал времени ( в секундах ), в течении которого широковещательное сообщение должно
 *  охватить всю сеть. Допустимый диапазон от nwkPassiveAckTimeout*nwkBroadcastRetries до 0xff.
 * */
extern uint8_t  nwkNetworkBroadcastDeliveryTime;

/*! @var nwkReportConstantCost
 *      Если атрибут установлен в FALSE, то сетевой уровень должен вычислять стоимость связей для всех
 *  своих соседей на основе LQI, полученного от канального уровня, иначе должен возвращать константное
 *  значение. Допустимые значение FALSE и TRUE.
 * */
extern bool_t  nwkReportConstantCost;

/*! @var nwkRouteDiscoveryRetriesPermitted
 *      Число повторных попыток отправки запроса на поиск маршрута после первой неудачной.
 *  Допустимый диапазон значений от 0x00 до 0x03.    
 * */
extern uint8_t  nwkRouteDiscoveryRetriesPermitted;

/*! @var nwkSymLink
 *      Если атрибут равен в TRUE, то установленный маршрут считается симметричным.
 *  То есть можно считать, что установлен маршрут как в одну сторону, так и в другую.
 *  Иначе нужно считать, что установлен маршрут только в одну сторону.
 * */
extern bool_t  nwkSymLink;

/*! @var nwkCapabilityInformation
 *      Атрибут содержит информацию о возможностях устройства. 
 * */
extern cap_info_t  nwkCapabilityInformation;

/*! @var nwkUseTreeAddrAlloc
 *      1 - использовать распределённую раздачу адресов,
 *      0 - разрешить вышележащему уровню самому раздавать адреса
 * */
extern bool_t  nwkUseTreeAddrAlloc; 

/*! @var nwkUseTreeRouting
 *      1 - использовать древовидную маршрутизацию,
 *      0 - не использовать её.
 * */
extern  bool_t nwkUseTreeRouting;

/*! @var nwkNextAddress
 *      Следующий адрес, который должен быть выделен при ассоциации.    
 *  Допустимый диапазон значений атрибута от 0 до 0xfffd.
 * */
extern  uint16_t    nwkNextAddress;

/*! @var nwkAvailableAddress
 *      Число оставшихся адресов, которые должны быть выделены при ассоциации.
 *  Допустимый диапазон значений атрибута от 0 до 0xfffd.
 * */
extern  uint16_t    nwkAvailableAddress;

/*! @var nwkAddressIncrement
 *      Значение, на которое увеличивается nwkNextAddress каждый раз при выделении адреса.
 *  Допустимый диапазон значений атрибута от 0 до 0xfffd.
 * */
extern  uint16_t    nwkAddressIncrement;

/*! @var    nwkTransactionPersistenceTime
 *      Максимальное время ( в суперфреймах ), в течении которого пакет транзакции хранится
 *  на родительском узле. допустимый диапазон значений от 0x0000 до 0xffff.
 * */
extern  uint16_t    nwkTransactionPersistenceTime;

/*! @var nwkShortAddress
 *      16-битный адрес, который устройство используется для работы в сети.
 * */
extern  uint16_t    nwkShortAddress;

/*! @var nwkStackProfile
 *      Идентификатор профайла zigbee-стека, используемого данным устройством.   
 *  Допустимый диапазон значений от 0 до 0x0f.
 * */
extern uint8_t  nwkStackProfile;

/*! @var    nwkProtocolVersion
 *      Версия протокола ZigBee, используемая в данный момент сетевым уровнем.
 *  Допустимый диапазон значений от 0 до 0x0f.
 * */
extern uint8_t  nwkProtocolVersion;

/*! @var nwkExtendedPANID
 *      Расширенный идентификатор сети, в которую входит данный узел.
 *  Значение 0 означает, что расширенный идентификатор сети неизвестен.
 *  Допустимый диапазаон значений атрибута от 0x0000000000000000 до 0xfffffffffffffffe.
 * */
extern uint64_t nwkExtendedPANID;

typedef  struct {
    znode_type_t    type    :2;     /* Текущее состояние данного устройства */
    unsigned        is_joined   :1; /* Присоединены ли мы к сети: 1-да,0-нет */
    unsigned        channel     :5; /* Текущий канал узла */
    unsigned        depth       :4; /* Глубина узла в сети */
    unsigned                    :4;
} nwk_extra_attr_t;

/*! @var nwkExtraAttr;
 *      Дополнительные атрибуты, содержит информацию о текущем состоянии сетевого уровня.
 * */
extern  nwk_extra_attr_t  nwkExtraAttr;

/*  @}  */
#endif  /*  _ZIGBEE_NETWORK_ATTR_H   */

