//  vim:encoding=utf-8
/*! @file  znet_field.c
 *  @brief Реализация доступа к полям сетевых пакетов
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <debug.h>
#include    <ztypes.h>
#include    <znet_field.h>

/* Получение доступа к битам из поля frame control */
/* Флаг группового пакета. Если флаг установлен в 1, то в пакете есть поле управления
 * групповой передачей */
#define     MULTICAST_MASK 0x01
#define     MULTICAST_FLAG(frame) ((frame[1] & MULTICAST_MASK)==MULTICAST_MASK)

/* Флаг маршрутизации от источника. Если флаг установлен в 1, то в пакете есть подфрейм
 * маршрутизации от источника */
#define     SRC_ROUTE_MASK 0x04
#define     SRC_ROUTE_FLAG(frame) ((frame[1] & SRC_ROUTE_MASK)==SRC_ROUTE_MASK)

/* Флаг наличия длинного адреса назначения в заголовке пакета */
#define     DST_EADDR_MASK 0x08
#define     DST_EADDR_FLAG(frame) ((frame[1] & DST_EADDR_MASK)==DST_EADDR_MASK)

/* Флаг наличия длинного адреса источника в заголовке пакета */
#define     SRC_EADDR_MASK 0x10
#define     SRC_EADDR_FLAG(frame) ((frame[1] & SRC_EADDR_MASK)==SRC_EADDR_MASK)

/* Размер в октетах неизменяемой части сетевого заголовка */
#define     CONST_HEAD_SIZE     (FRAME_CTRL_SIZE+DST_ADDR_SIZE+SRC_ADDR_SIZE+RADIUS_SIZE+SEQ_NUM_SIZE)
/* Общее число полей сетевого пакета, обрабатываемых данным модулем */
#define     TOTAL_NET_FIELD     12

/* Размеры полей, проиндексированные в соответствии с номерами полей */
const   uint8_t     net_field_size[ TOTAL_NET_FIELD ] = {
    FRAME_CTRL_SIZE, DST_ADDR_SIZE, SRC_ADDR_SIZE, RADIUS_SIZE, SEQ_NUM_SIZE,
    DST_EADDR_SIZE, SRC_EADDR_SIZE, MULTICAST_CTRL_SIZE, RELAY_COUNT_SIZE, RELAY_INDEX_SIZE,
    RELAY_LIST_SIZE, PAYLOAD_SIZE
};

/* Функция, возвращающая по номеру поля указатель на него в пакете и его размер */
void*  znet_field_info(  uint8_t *frame, uint8_t num, uint16_t* len_ptr )
{
    ASSERT((uint16_t *)0 == len_ptr, "Error: length pointer is NULL" );
    ASSERT( TOTAL_NET_FIELD <= num, "Error: invalid number of network field" );
    *len_ptr = net_field_size[num]; /* Получение размера поля */
    ASSERT((uint8_t *)0 == frame, "Error: frame pointer is NULL");

    /* Определение смещения поля по его номеру в данном пакете */
    switch( num ) {
        case FRAME_CTRL: return frame;
        case DST_ADDR:   return frame+FRAME_CTRL_SIZE;
        case SRC_ADDR:   return frame+FRAME_CTRL_SIZE+DST_ADDR_SIZE;
        case RADIUS:     return frame+FRAME_CTRL_SIZE+DST_ADDR_SIZE+SRC_ADDR_SIZE;
        case SEQ_NUM:    return frame+FRAME_CTRL_SIZE+DST_ADDR_SIZE+SRC_ADDR_SIZE+RADIUS_SIZE;
        case DST_EADDR:  return DST_EADDR_FLAG(frame)?frame+CONST_HEAD_SIZE:0;
        case SRC_EADDR: 
            if( SRC_EADDR_FLAG(frame) ) {
                frame += CONST_HEAD_SIZE;
                return frame + (DST_EADDR_FLAG(frame)?DST_EADDR_SIZE:0);    
            } else break;
        case MULTICAST_CTRL:
            if( MULTICAST_FLAG(frame) ) { 
                frame += CONST_HEAD_SIZE;
                frame += DST_EADDR_FLAG(frame)? DST_EADDR_SIZE:0;
                return frame + (SRC_EADDR_FLAG(frame)? SRC_EADDR_SIZE:0);
            } else break;
        case RELAY_COUNT: case RELAY_INDEX: 
            if( SRC_ROUTE_FLAG(frame) ) {
                frame += CONST_HEAD_SIZE;
                frame += DST_EADDR_FLAG(frame)? DST_EADDR_SIZE:0;
                frame += SRC_EADDR_FLAG(frame)? SRC_EADDR_SIZE:0;
                frame += MULTICAST_FLAG(frame)? MULTICAST_CTRL_SIZE:0;
                return (RELAY_COUNT == num)?frame: frame+RELAY_COUNT_SIZE;
            } else break;
        case  RELAY_LIST:
            if( SRC_ROUTE_FLAG(frame) ) {
                frame += CONST_HEAD_SIZE;
                frame += DST_EADDR_FLAG(frame)? DST_EADDR_SIZE:0;
                frame += SRC_EADDR_FLAG(frame)? SRC_EADDR_SIZE:0;
                frame += MULTICAST_FLAG(frame)? MULTICAST_CTRL_SIZE:0;
                return frame+RELAY_COUNT_SIZE+RELAY_INDEX_SIZE;
            } else break;
        case  PAYLOAD:
            frame += CONST_HEAD_SIZE;
            frame += DST_EADDR_FLAG(frame)? DST_EADDR_SIZE:0;
            frame += SRC_EADDR_FLAG(frame)? SRC_EADDR_SIZE:0;
            frame += MULTICAST_FLAG(frame)? MULTICAST_CTRL_SIZE:0;
            if( SRC_ROUTE_FLAG(frame) ) 
                frame += RELAY_COUNT_SIZE+RELAY_INDEX_SIZE + (*frame)*sizeof(s_addr_t);
            return frame;
        default: break;
    }
    return (void *)0;
}

