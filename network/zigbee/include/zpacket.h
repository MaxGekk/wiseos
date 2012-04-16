//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_PACKET_H
#define     _ZIGBEE_NETWORK_PACKET_H
/*! @file  zpacket.h
 *  @brief Интерфейс операций с пакетами
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <types.h>
#include    <zphy_const.h>

/*! @defgroup ZIGBEE_NETWORK_PACKET Управление пакетами
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 * */

/* Максимальное число созданных пакетов */
#if !defined( Z_MAX_PACKET )
#define Z_MAX_PACKET    4
#endif

/* Структура  пакета стека ZigBee */
typedef struct {
    unsigned    busy        :1;     /*!< Флаг занятости пакета */
    unsigned    reserved    :7;
    uint8_t     lqi;                /*!< Качество связи при приёме пакета */
    uint32_t    time_stamp;         /*!< Время приёма пакета */
    uint8_t     pos;                /*!< Смещение тела пакета в массиве data */
    uint8_t     len;                /*!< Размер тела пакета */
    uint8_t     data[ aMaxPHYPacketSize ];  /*!< Поле, содержащее тело пакета */
} packet_t;

/*! @fn packet_t*   zpkt_new();
 *  @brief Выделение места в пуле под новый пакет.
 *  @return Возвращается указатель на выделенный пакет, иначе 0.
 * */
packet_t*   zpkt_new();

/*! @fn uint8_t*    zpkt_append( packet_t *pkt_ptr, uint8_t len );
 *  @brief  Добавление к телу пакета len октетов "спереди".
 *  @param pkt_ptr - указатель на пакет
 *  @param len - размер, на который увеличивается тело пакета
 *  @return Возвращается указатель на вновь добавленную к телу пакета область,
 *  или 0 в случае неудачи.
 * */
uint8_t*    zpkt_append( packet_t *pkt_ptr, uint8_t len );

/*! @fn uint8_t*    zpkt_cut( packet_t  *pkt_ptr, uint8_t len );
 *  @brief Отрезание от тела пакета len октетов "спереди".
 *  @param pkt_ptr - указатель на пакет
 *  @param len - размер отрезаемой области.
 *  @return Возвращается указатель на начало тела пакета после отрезания
 *  len октетов "спереди", или 0 в случае неудачи.
 * */
uint8_t*    zpkt_cut( packet_t  *pkt_ptr, uint8_t len );

/*! @fn result_t    zpkt_free( packet_t *pkt_ptr )
 *  @brief Освобождение пакета и возвращение используемой им памяти в пул пакетов.
 *  @param pkt_ptr - указатель на пакет.
 * */
result_t    zpkt_free( packet_t *pkt_ptr );

/*! @fn   uint8_t*    zpkt_body( packet_t *pkt_ptr );
 *  @brief  Функция возвращает указатель на начало пакета
 *  @param pkt_ptr - указатель на пакет
 *  @return Возвращается указатель на начало тела пакета или 0 в случае ошибки.
 * */
uint8_t*    zpkt_body( packet_t *pkt_ptr );

/*  @}  */
#endif      /* _ZIGBEE_NETWORK_PACKET_H */

