//  vim:encoding=utf-8
#ifndef     _ZIGBEE_PHY_CONST_H
#define     _ZIGBEE_PHY_CONST_H
/*! @file  zphy_const.h
 *  @brief Константы физического уровня
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

/*! @defgroup ZIGBEE_PHY_CONST Константы физического уровня
 *  @ingroup  ZIGBEE_PHY
 *  @{ 
 *      Константы определяют свойства ( характеристики ) физического уровня.
 * */

/*! @def aMaxPHYPacketSize
 *      Максимальный размер пакета физического уровня в октетах.
 * */
#define  aMaxPHYPacketSize  127

/*! @def aMaxPHYPacketSize
 *      Максимальное время ( в символах ) переключения между приёмом и передачей,
 *  и передачей и приёмом.
 * */
#define aTurnaroundTime     12

/*  @}  */
#endif  /* _ZIGBEE_PHY_CONST_H */

