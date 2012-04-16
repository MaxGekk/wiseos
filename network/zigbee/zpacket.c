//  vim:encoding=utf-8
/*! @file  zpacket.c
 *  @brief Операции с пакетами
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <mem.h>
#include    <debug.h>
#include    <ztypes.h>
#include    <zpacket.h>
#include    <znet_reset.h>

/* Пул пакетов */
packet_t    zpkt_pool[ Z_MAX_PACKET ];

/* Проверка корректности указателя на пакет */
#define CHECK_PTR( pkt_ptr ) \
    /* Проверка того, что указатель на пул пакетов */ \
    ASSERT1( ( pkt_ptr < zpkt_pool )||( &zpkt_pool[Z_MAX_PACKET] <= pkt_ptr ), \
            "Invalid pkt_ptr [%x] : it is out of packet pool", (uint16_t)pkt_ptr );\
    /* Проверка того, что указатель на пакет */ \
    ASSERT1( (pkt_ptr - zpkt_pool) % sizeof(packet_t), \
            "Invalid pkt_ptr [%x] : it is not point to packet", (uint16_t)pkt_ptr );

/* Выделение места под новый пакет в пуле */
packet_t*   zpkt_new()
{
    uint16_t i;
    /* Поиск свободного места в пуле */
    for( i=0; i<Z_MAX_PACKET; i++ )
        if( 0 == zpkt_pool[i].busy )
            break;

    /* Если пул полон, то возвращаем 0 */
    if( Z_MAX_PACKET <= i )
        return (packet_t *)0;

    /* Инициализируем поля пакета */
    zpkt_pool[i].busy = 1;
    zpkt_pool[i].lqi = 0;
    zpkt_pool[i].time_stamp = 0;
    zpkt_pool[i].pos = 0;
    zpkt_pool[i].len = 0;

    return &zpkt_pool[i];
}

/* Освобождение места в пуле, занятого пакетом */
result_t    zpkt_free( packet_t *pkt_ptr )
{
    CHECK_PTR( pkt_ptr );

    pkt_ptr->busy = 0;

    return ENOERR;
}

/* Получение указателя на тело пакета */
inline uint8_t*    zpkt_body( packet_t *pkt_ptr )
{
    CHECK_PTR( pkt_ptr );

    return (uint8_t *)&(pkt_ptr->data[pkt_ptr->pos]);
}

/* Резервирование в начале пакета памяти определённого размера */
uint8_t*    zpkt_append( packet_t *pkt_ptr, uint8_t len )
{
    CHECK_PTR( pkt_ptr );

    ASSERT1( 0 == len, "Warning: added 0 octets to packet [%x]", (uint16_t)pkt_ptr );

    if( 0 == pkt_ptr->busy )
        return (uint8_t *)0;

    /* Есть ли в начале пакета достаточный объём свободной памяти? */
    if( ( aMaxPHYPacketSize - pkt_ptr->len ) < len )
        return (uint8_t *)0;

    if( 0 == pkt_ptr->len ) {
        pkt_ptr->len = len;
        pkt_ptr->pos = aMaxPHYPacketSize - len;
    } else {
        pkt_ptr->pos -= len;
        pkt_ptr->len += len;
    }

    return zpkt_body( pkt_ptr );
}

/* Откусывание от начала пакета определённого куска памяти */
uint8_t*    zpkt_cut( packet_t  *pkt_ptr, uint8_t len )
{
    CHECK_PTR( pkt_ptr );

    ASSERT1( 0 == len, "Warning:  cut 0 octets from packet [%x]", (uint16_t)pkt_ptr );

    if( 0 == pkt_ptr->busy )
        return (uint8_t *)0;

    /* Откусываемый кусок больше, чем размер пакета . Возвращаем ошибку, поскольку
     * вероятнее всего ошибка где-то в пользовательской ф-ции. */
    if( pkt_ptr->len < len )
        return (uint8_t *)0;
  
    pkt_ptr->len -= len;
    pkt_ptr->pos += len;

    return zpkt_body( pkt_ptr );
}

/* Сброс пула пактов */
void    reset_packet_pool()
{
    mset( zpkt_pool, sizeof(packet_t)*Z_MAX_PACKET, 0 );
}

REG_RESET( reset_packet_pool );

