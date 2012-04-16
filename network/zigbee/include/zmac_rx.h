//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_RX_H
#define     _ZIGBEE_MAC_RX_H
/*! @file  zmac_rx.h
 *  @brief Интерфейс приёма данных
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

/*! @defgroup ZIGBEE_MAC_DATA_RX  Интерфейс приёма фреймов
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 * */

/* Аргументы стековых вызовов приёма данных */
typedef     struct {
    packet_t    *pkt;   /*!< Указатель на принятый фрейм */
    uint8_t     dsn;    /*!< Последовательный номер фрейма */
    uint8_t     src_mode;
    s_addr_t    saddr;
    e_addr_t    eaddr;
} zargs_mdata_rx_t;

/* Имя стекового вызова для приёма фреймов от канального уровня */
#define     ZCALL_MAC_DATA_RX 111

/*! @}  */

#endif      /* _ZIGBEE_MAC_RX_H */

