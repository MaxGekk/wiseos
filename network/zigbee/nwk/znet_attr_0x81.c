//  vim:encoding=utf-8
/*! @file  znet_attr_0x81.c
 *  @brief Атрибут - последовательный номер пакетов
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    2           
 *  */

#include    <types.h>
#include    <math.h>
#include    <zmac_const.h>
#include    <znet_reset.h>

#define  XOR_EADDR  ( (aExtendedAddress>>8) ^ (aExtendedAddress>>16) \
        ^ (aExtendedAddress>>24) ^ (aExtendedAddress>>32) \
        ^ (aExtendedAddress>>40) ^ (aExtendedAddress>>48) ^ (aExtendedAddress>>56) )

/* Последовательный номер пакетов, осозданных на узле. Увеличивается для каждого 
 * следующего пакета. */
uint8_t nwkSequenceNumber = XOR_EADDR;

/* Сброс атрибута. 
 * TODO: Нужно ещё учесть текущее время при переинициализации атрибута
 * */
void    znet_reset_a0x81()
{
    nwkSequenceNumber = random8() ^ XOR_EADDR;
}

REG_RESET( znet_reset_a0x81 );

