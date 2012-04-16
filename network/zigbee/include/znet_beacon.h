//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_BEACON_H
#define     _ZIGBEE_NETWORK_BEACON_H
/*! @file  znet_beacon.h
 *  @brief Функции обработки сетевой части кадра-маяка
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zfield.h>

/*! @defgroup ZIGBEE_NET_BEACON Обработка сетевой части кадра-маяка
 *  @ingroup  ZIGBEE_NET
 *  @{
 */

/*! @def    GET_PROTOCOL_ID(bcn_ptr)
 *  @brief  Макрос возвращает идентификатор протокола
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 * */
#define     GET_PROTOCOL_ID(bcn_ptr) \
    OCTET(bcn_ptr,0)

/*! @def SET_PROTOCOL_ID(bcn_ptr,val)
 *  @brief Установка идентификатора протокола
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  val - новый идентификатор протокола
 * */
#define     SET_PROTOCOL_ID(bcn_ptr,val)  \
    ({ OCTET(bcn_ptr,0) = val; })

/*! @def GET_STACK_PROFILE(bcn_ptr)
 *  @brief Получение профиля стека из кадра-маяка
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 * */
#define     GET_STACK_PROFILE(bcn_ptr) \
    (OCTET(bcn_ptr,1) & 0x0f)

/*! @def SET_STACK_PROFILE(bcn_ptr,val)
 *  @brief Установка профиля стека
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  val - значение профиля стека
 * */
#define     SET_STACK_PROFILE(bcn_ptr,val) \
    ({ OCTET(bcn_ptr,1) &= ~0x0f; OCTET(bcn_ptr,1) |= val&0x0f; })

/*! @def GET_PROTOCOL_VERSION(bcn_ptr)
 *  @brief Получение версии протокола
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 * */
#define     GET_PROTOCOL_VERSION(bcn_ptr) \
    ((OCTET(bcn_ptr,1)>>4) & 0x0f)   

/*! @def SET_PROTOCOL_VERSION(bcn_ptr,val)
 *  @brief Установка версии протокола
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  val - версия протокола
 * */
#define     SET_PROTOCOL_VERSION(bcn_ptr,val) \
    ({ OCTET(bcn_ptr,1) &= ~0xf0; OCTET(bcn_ptr,1) |= (val<<4) & 0xf0; })

/*! @def GET_ROUT_CAPACITY(bcn_ptr)
 *  @brief Может ли узел подключать к себе маршрутизаторы
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @return Возвращается 1 если узел может присоединять к себе маршрутизаторы, иначе 0.
 * */
#define     GET_ROUT_CAPACITY(bcn_ptr) \
    ((OCTET(bcn_ptr,2)&0x04)==0x04)

/*! @def SET_ROUT_CAPACITY(bcn_ptr, val)
 *  @brief Установка флага возможности присоединения маршрутизаторов
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  val - 1 - если узел может присоединять к себе маршрутизаторы, в противном
 *  случае 0.
 * */
#define     SET_ROUT_CAPACITY(bcn_ptr, val) \
    ({OCTET(bcn_ptr,2) &= ~0x04;OCTET(bcn_ptr,2) |= (val&0x04);})

/*! @def GET_DEPTH(bcn_ptr) 
 *  @brief Получение глубины устройства в сети относительно координатора
 *  Глубина самого координатора равна 0.
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 * */
#define     GET_DEPTH(bcn_ptr) \
    ((OCTET(bcn_ptr,2)>>3) & 0x0f)

/*! @def SET_DEPTH(bcn_ptr, val)
 *  @brief Установка глубины устройства в сети.
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  val - глубина устройства в сети
 *  случае 0.
 * */
#define     SET_DEPTH(bcn_ptr, val) \
    ({OCTET(bcn_ptr,2) &= ~(0x0f<<3); OCTET(bcn_ptr,2) |= ((val & 0x0f)<<3); })

/*! @def GET_ROUT_CAPACITY(bcn_ptr)
 *  @brief Может ли узел подключать к себе конечные узлы
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @return Возвращается 1 если узел может присоединять к себе конечные узлы, иначе 0.
 * */
#define     GET_END_CAPACITY(bcn_ptr) \
    ((OCTET(bcn_ptr,2) & 0x80)==0x80)

/*! @def SET_ROUT_CAPACITY(bcn_ptr, val)
 *  @brief Установка флага возможности присоединения конечных узлов
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  val - 1 - если узел может присоединять к себе конечные узлы, в противном
 *  случае 0.
 * */
#define     SET_END_CAPACITY(bcn_ptr, val) \
    ({OCTET(bcn_ptr,2) &= ~0x80; OCTET(bcn_ptr,2) |= (val & 0x80);})

/*! @def GET_EPANID(bcn_ptr,to)
 *  @brief Вычитывание из кадра-маяка расширенного идентификатора сети
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  to - указатель на область памяти, в которую должен быть скопирован расширенный
 *  идентификатор сети
 * */
#define     GET_EPANID(bcn_ptr,to) \
    reorder( bcn_ptr, 3, to, 0, sizeof(*to) )

/*! @def    SET_EPANID(bcn_ptr, from)
 *  @brief Запись расширенного идентификатора сети в кадр-маяк
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  from - указатель на область памяти, из которой должен быть взят
 *  расширенный идентификатор сети
 * */
#define     SET_EPANID(bcn_ptr, from) \
    reorder( from, 0, bcn_ptr, 3, sizeof(*from) )

/*! @def    GET_TX_OFFSET( bcn_ptr, to)
 *  @brief Вычитывание из кадра-маяка смещения родительского кадра-маяка
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  to - указатель на область памяти, в которую должено быть скопировано
 *  смещение родительского кадра-маяка 
 * */
#define     GET_TX_OFFSET( bcn_ptr, to) \
    reorder( bcn_ptr, 11, to, 0, sizeof(*to) )

/*! @def    SET_TX_OFFSET( bcn_ptr, from )
 *  @brief Запись смещения родительского кадра-маяка
 *  @param  bcn_ptr - указатель на начало полезной нагрузки кадра-маяка
 *  @param  from - указатель на область памяти, из которой должено быть взято
 *  смещение родительского кадра-маяка
 * */
#define     SET_TX_OFFSET( bcn_ptr, from ) \
    reorder( from, 0, bcn_ptr, 11, sizeof(*from) )

/*! @fn void    bcn_all_cap_off();
 *  @brief  Запрещение присоединения любых типов устройств.
 *      Вызов функции приводит к сбросу соответствующих битов в кадре-маяке.
 * */
void    bcn_all_cap_off();

/*! @fn  void     bcn_cap_off( uint8_t dev_type );
 *  @brief Сброс в кадре-маяке битов разрешения подключения роутеров либо конечных устройств.
 *  @param dev_type - тип устройства: FFD - полнофункциональное ( роутер ), RFD - конечное устройство.
 *  */
void     bcn_cap_off( uint8_t dev_type );

/*! @fn  void    bcn_cap_on( uint8_t dev_type );
 *  @brief Установка в кадре-маяке битов разрешения подключения роутеров либо конечных устройств.
 *  @param dev_type - тип устройства: FFD - полнофункциональное ( роутер ), RFD - конечное устройство.
 *  */
void    bcn_cap_on( uint8_t dev_type );

/*!  @}  */
#endif  /*  _ZIGBEE_NETWORK_BEACON_H */

