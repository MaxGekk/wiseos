#include   <mem.h>

void    memset( void *dst, size_t len, uint8_t c )
{
    const uint8_t *const after_end = (uint8_t *)dst + len;
    while( dst < after_end )
        *((uint8_t *)dst)++ = c;
}

