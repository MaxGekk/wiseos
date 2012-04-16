//  vim:encoding=utf-8
/*! @file  znet_attr_0x83.c
 *  @brief Атрибут - максимальное число повторных отправок широковещательных пакетов
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* По умолчанию 3 попытки */
#define     DEFAULT_MAX_BROADCAST_RETRIES   0x03

/* После неудачной отправки широковещательного пакета, сетевой уровень
 * должен попытаться ещё раз. Максимальное число таких попыток определяется
 * данным атрибутом. */
uint8_t  nwkMaxBroadcastRetries = DEFAULT_MAX_BROADCAST_RETRIES;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x83()
{
    nwkMaxBroadcastRetries = DEFAULT_MAX_BROADCAST_RETRIES;
}

REG_RESET( znet_reset_a0x83 );

