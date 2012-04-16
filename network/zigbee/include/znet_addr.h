//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_ADDR_H
#define     _ZIGBEE_NETWORK_ADDR_H
/*! @file  znet_addr.h
 *  @brief Интерфейс выделения адресов
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup ZIGBEE_NETWORK_ADDR Выделение нового адреса
 *  @ingroup  ZIGBEE_NETWORK
 *  @{
 */

/*! @fn s_addr_t    znet_addr_alloc( uint8_t depth, uint8_t is_ffd );
 *  @brief Выделение нового адреса для дочернего узла
 *  @param dev_type - 1 - если полнофункциональное устройство, иначе 0.
 *  @return В случае успеха возвращается новый адрес, иначе 0xffff.
 * */
s_addr_t    znet_addr_alloc( uint8_t dev_type );

/*! @} */
#endif  /*  _ZIGBEE_NETWORK_ADDR_H */

