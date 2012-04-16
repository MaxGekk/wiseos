/*! @file  power.c
 *  @brief Возведение в степень
 *  @author     Max Gekk
 *  @date       сентябрь 2007
 *  @version    1
 *  */

#include   <math.h>

uint16_t power( uint8_t a, uint8_t n )
{
    int p = 1, t = a;
    for(;;) {
        if( n&1 )
            p *= t;
        n >>= 1;
        if( n ) t *= t;
        else break;
    }
    return p;
}

