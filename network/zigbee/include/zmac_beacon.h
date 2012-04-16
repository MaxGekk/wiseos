//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_BEACON_H
#define     _ZIGBEE_MAC_BEACON_H
/*! @file  zmac_beacon.h
 *  @brief Доступ к полезной нагрузке кадра-маяка узла
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup ZIGBEE_MAC_BEACON Доступ к полезной нагрузке кадра-маяка
 *  @ingroup  ZIGBEE_MAC
 *  @{
 */

/*! @fn void    bcn_payload_get( void   *to );
 *  @brief Получение полезной нагрузки кадра-маяка
 *  @param to - указатель, по которому необходимо записать полезную
 *  нагрузку кадра-маяка
 * */
void    bcn_payload_get( void   *to );

/*! @fn void    bcn_payload_set( void   *from );
 *  @brief  Обновить полезную нагрузку кадра-маяка
 *  @param from - указатель на новую полезную нагрузку
 * */
void    bcn_payload_set( void   *from );

/*!  @}  */
#endif  /*  _ZIGBEE_MAC_BEACON_H */

