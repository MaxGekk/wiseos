//  vim:encoding=utf-8
/*! @file  znet_attr_0x81.c
 *  @brief Атрибут - таймаут пассивного подтверждения
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */
#include    <ztypes.h>
#include    <znet_reset.h>

/* По умолчанию 3 секунды*/
#define     DEFAULT_PASSIVE_ACK_TIMEOUT     0x03

/* Интервал времени в секундах, в течении которого нужно ожидать
 * пересылки нашего широковещательного пакета всеми детьми и 
 * родительским узлом. */
uint8_t  nwkPassiveAckTimeout = DEFAULT_PASSIVE_ACK_TIMEOUT;

/* Сброс атрибута в значение по умолчанию. */
void    znet_reset_a0x82()
{
    nwkPassiveAckTimeout = DEFAULT_PASSIVE_ACK_TIMEOUT;
}

REG_RESET( znet_reset_a0x82 );

