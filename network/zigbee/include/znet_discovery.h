//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_DISCOVERY_H
#define     _ZIGBEE_NETWORK_DISCOVERY_H
/*! @file  znet_discovery.h
 *  @brief Интерфейс обнаружения соседних сетей
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zneighbor.h>
#include    <zmac_scan.h>

/*! @defgroup ZIGBEE_NET_DISCOVERY Обнаружение соседних сетей
 *  @ingroup  ZIGBEE_NET
 *  @{
 */

/*! @def  ZCALL_NET_FORMATION   
 *  @brief Имя стекового вызова для поиска соседних сетей
 *  */
#define     ZCALL_NET_DISCOVERY   5

/*! @typedef  zargs_ndisc_t 
 *  @brief Аргументы стекового вызова ZCALL_NET_DISCOVERY
 *  @param channels -  Вх. Битовая маска каналов, на которых необхидимо  осуществлять поиск
 *  @param duration -  Вх. Аргумент определяющий продолжительность сканирования.
 *      Каждый канал должен сканироваться aBaseSuperframeDuration*(2^duration+1) символов
 *  @param status - Вых. Статус завершения операции обнаружения соседних сетей.
 *  */
typedef     zargs_ascan_t   zargs_ndisc_t;

/*! @typedef  network_t
 *  @brief Тип описателя сети
 *  @param s_panid - короткий идентификатор сети (16 бит)
 *  @param e_panid - длинный идентификатор сети (64 бита)
 *  @param channel - номер логического канала, на котором была обеаружена сеть
 *  @param beacon_order - параметр, определяющий период отправки кадров-маяков
 *  @param sf_order - параметр, задающий длину активной части суперфрейма
 * */
typedef     neighbor_t  network_t;

#define     IS_NET_VALID( nwk_ptr )     IN_TABLE( nwk_ptr )

/*! @fn network_t*   net_next( network_t  *network, uint16_t  *perm_join );
 *  @brief Функция возвращает описатель следующей сети
 *  @param nwk_ptr - указатель на описатель текущей сети.
 *  @param perm_join - указатель на область памяти, в которую будет записана 1
 *  если разрешено присоединение к сети, и 0 - в противном случае.
 *  @return Возвращается описатель следующей сети. Если в качестве
 *  аргумента передать 0, то возвращается первый описатель сети. 
 * */
network_t*   net_next( network_t  *nwk_ptr, uint16_t  *perm_join );

/*!  @}  */
#endif  /*  _ZIGBEE_NETWORK_DISCOVERY_H */

