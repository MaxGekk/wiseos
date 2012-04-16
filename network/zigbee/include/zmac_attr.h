//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_ATTR_H
#define     _ZIGBEE_MAC_ATTR_H
/*! @file  zmac_attr.h
 *  @brief Атрибуты канального уровня 
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup  ZIGBEE_MAC_ATTR Атрибуты канального уровня
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 *      Атрибуты канального уровня IEEE 802.15.4 определяют внутреннее состояние
 *  уровня, а также позволяют влиять на работу последнего.
 * */

/*! @var macAssociationPermit
 *  Разрешение/запрещение присоединения дочерних узлов. Если атрибут равен TRUE, то
 *  ассоциация разрешена, иначе нет.
 *  * */
extern bool_t  macAssociationPermit;

/*! @var macBeaconOrder
 *  Атрибут, определяющий режим работы узла. Если macBeaconOrder равен 15, то узел
 *  работает в режиме без кадров-маяков. Если 0<=macBeaconOrder<15, то узел работает
 *  в режиме с кадрами-маяками. 
 *  * */
extern uint8_t  macBeaconOrder;

/*! @var macPANID
 *  @brief  Идентификатор сети
 * */
extern s_panid_t    macPANID;

/*! @var macShortAddress
 *  @brief Короткий адрес устройства
 * */
extern s_addr_t     macShortAddress;

/*! @var macAutoRequest
 *  Запрашивать ли автоматически данные с координатора
 * */
extern bool_t   macAutoRequest;

/*! @var macRxOnWhenIdle
 *  Переходить ли на приём в периоды бездействия
 * */
extern bool_t   macRxOnWhenIdle;

/*  @}  */
#endif  /*  _ZIGBEE_NETWORK_ATTR_H   */

