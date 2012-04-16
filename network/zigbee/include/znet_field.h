//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_FIELD_H
#define     _ZIGBEE_NETWORK_FIELD_H
/*! @file  znet_field.h
 *  @brief Описание полей пакетов сетевого уровня
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_const.h>
#include    <zfield.h>

/*! @defgroup ZIGBEE_NETWORK_FIELD Доступ к полям пакетов сетевого уровня
 *  @ingroup  ZIGBEE_FIELD
 *  @{
 *  */

/*! @fn void*  znet_field_info(  uint8_t *frame, uint8_t num, uint16_t* len_ptr );
 *  @brief Функция возвращающая по порядковому номеру поля указатель на него, а также
 *  его размер.
 *  @param frame - указатель на начало фрейма
 *  @param num - порядковый номер поля
 *  @param len_ptr - указатель на область памяти, в которую должен быть записан размер поля
 *  @return Функция возвращает указатель на поле в случае успеха, иначе 0.
 * */
void*  znet_field_info(  uint8_t *frame, uint8_t num, uint16_t* len_ptr );

/* Инициализация поля сетевого пакета */
#define     NET_FIELD_INIT(field, frame) FIELD_INIT(field, frame, znet_field_info)

/* Информация о полях сетевого пакета и доступ к ним */
/* Управляющее поле сетевого пакета */
#define     FRAME_CTRL  0
#define     FRAME_CTRL_TYPE     uint16_t
#define     FRAME_CTRL_SIZE     sizeof(FRAME_CTRL_TYPE)
#define     GET_FRAME_CTRL(field)   GET_FIELD(field, FRAME_CTRL)
#define     SET_FRAME_CTRL(field, value)    SET_FIELD(field, FRAME_CTRL, value)

#define     FC_TYPE_MASK    3   /*!< Маска поля тип фрейма */
#define     FC_DATA     0x00    /*!< Тип фрейма - данные */
#define     FC_CMD      0x01    /*!< Тип фрейма = команда */
#define     FC_PROTOCOL_VER   (( nwkcProtocolVersion & 0x0f ) << 2)     /*!< Версия протокола */
#define     FC_DR_SUPPRESS  (0<<6)    /* подавить обнаружение маршрута */
#define     FC_DR_ENABLE    (1<<6)    /* включить обнаружение маршрута */  
#define     FC_DR_DISABLE   (2<<6)    /* Отключить обнаружение маршрута */
#define     FC_MULTICAST    (1<<8)    /* Групповая передача */
#define     FC_SEC          (1<<9)    /* Включение криптозащиты */
#define     FC_SRC_ROUT     (1<<10)   /* Маршрутизация от источника */  
#define     FC_DADDR        (1<<11)   /* Включить поле с длинным адресом назначения */  
#define     FC_SADDR        (1<<12)   /* Включить поле с длинным адресом источника */  


/* Сетевой адрес назначения */
#define     DST_ADDR    1
#define     DST_ADDR_TYPE       uint16_t
#define     DST_ADDR_SIZE       sizeof(DST_ADDR_TYPE)
#define     GET_DST_ADDR(field)     GET_FIELD(field, DST_ADDR)
#define     SET_DST_ADDR(field, value)  SET_FIELD(field, DST_ADDR, value)

/* Сетевой адрес источника */
#define     SRC_ADDR    2
#define     SRC_ADDR_TYPE       uint16_t
#define     SRC_ADDR_SIZE       sizeof(SRC_ADDR_TYPE)
#define     GET_SRC_ADDR(field) GET_FIELD(field, SRC_ADDR )
#define     SET_SRC_ADDR(field, value)  SET_FIELD(field, SRC_ADDR, value)

/* Радиус распространения пакета */
#define     RADIUS      3
#define     RADIUS_TYPE     uint8_t
#define     RADIUS_SIZE         sizeof(RADIUS_TYPE)
#define     GET_RADIUS(field)   GET_FIELD(field, RADIUS )
#define     SET_RADIUS(field, value)    SET_FIELD(field, RADIUS, value)

/* Последовательный номер сетевого пакета */
#define     SEQ_NUM     4
#define     SEQ_NUM_TYPE    uint8_t
#define     SEQ_NUM_SIZE        sizeof(SEQ_NUM_TYPE)
#define     GET_SEQ_NUM(field)  GET_FIELD(field, SEQ_NUM)
#define     SET_SEQ_NUM(field, value)   SET_FIELD(field, SEQ_NUM, value)

/* Длинный адрес назначения */
#define     DST_EADDR    5
#define     DST_EADDR_TYPE  uint64_t
#define     DST_EADDR_SIZE      sizeof(DST_EADDR_TYPE)
#define     GET_DST_EADDR(field)    GET_FIELD(field, DST_EADDR)
#define     SET_DST_EADDR(field, value)    SET_FIELD(field, DST_EADDR, value)

/* Длинный адрес источника */
#define     SRC_EADDR   6
#define     SRC_EADDR_TYPE  uint64_t
#define     SRC_EADDR_SIZE      sizeof(SRC_EADDR_TYPE)
#define     GET_SRC_EADDR(field)    GET_FIELD(field, SRC_EADDR)
#define     SET_SRC_EADDR(field,value)  SET_FIELD(field, SRC_EADDR, value)

/* Управляющее поле групповой передачи */
#define     MULTICAST_CTRL  7
#define     MULTICAST_CTRL_TYPE uint8_t
#define     MULTICAST_CTRL_SIZE sizeof(MULTICAST_CTRL_TYPE)
#define     GET_MULTICAST_CTRL(field)   GET_FIELD(field, MULTICAST_CTRL)
#define     SET_MULTICAST_CTRL(field, value)    SET_FIELD(field, MULTICAST_CTRL, value)

/* Число узлов при маршрутизации от источника */
#define     RELAY_COUNT     8
#define     RELAY_COUNT_TYPE    uint8_t
#define     RELAY_COUNT_SIZE    sizeof(RELAY_COUNT_TYPE)
#define     GET_RELAY_COUNT(field)  GET_FIELD(field, RELAY_COUNT)
#define     SET_RELAY_COUNT(field,value)    SET_FIELD(field, RELAY_COUNT,value)

/* Текущий узел при маршрутизации от источника */
#define     RELAY_INDEX     9
#define     RELAY_INDEX_TYPE    uint8_t
#define     RELAY_INDEX_SIZE    sizeof(RELAY_INDEX_TYPE)
#define     GET_RELAY_INDEX(field)  GET_FIELD(field, RELAY_INDEX)
#define     SET_RELAY_INDEX(field,value)    SET_FIELD(field, RELAY_INDEX, value)

/* Список узлов при маршрутизации от источника */
#define     RELAY_LIST      10
#define     RELAY_LIST_TYPE    void
#define     RELAY_LIST_SIZE     ZNET_VAR_LEN
#define     GET_RELAY_LIST(field)   GET_FIELD(field, RELAY_LIST)

/* Полезная нагрузка сетевого пакета */
#define     PAYLOAD     11
#define     PAYLOAD_TYPE    void
#define     PAYLOAD_SIZE        ZNET_VAR_LEN
#define     GET_PAYLOAD(field)      GET_FIELD(field, PAYLOAD)

/* Идентификаторы командных фреймов */
#define     CMD_ROUTE_REQUEST       0x01
#define     CMD_ROUTE_REPLAY        0x02
#define     CMD_ROUTE_ERROR         0x03
#define     CMD_LEAVE               0x04
#define     CMD_ROUTE_RECORD        0x05
#define     CMD_REJOIN_REQUEST      0x06
#define     CMD_REJOIN_RESPONSE     0x07

/*!  @}  */
#endif      /* _ZIGBEE_FIELD_H */

