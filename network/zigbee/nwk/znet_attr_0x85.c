//  vim:encoding=utf-8
/*! @file  znet_attr_0x85.c
 *  @brief Атрибут - максимальная глубина сети
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* Максимальная глубина сети по умолчанию 5. Такая глубина 
 * получается, потому что по умолчанию число детей 7. 
 * Максимальную глубину можно подсчитать следующим образом:
    int     max_depth( int max_rout )
    {
        int space = 65536 - 1;
        int depth = 0;
        while ( max_rout <= space ) {
            space = space / max_rout;
            space -= 1;
            depth  += 1;
        }
        return depth;
    }
 * */
#define     DEFAULT_MAX_DEPTH   0x05

/* Максимальная глубина сети ( радиус сети ). У координатора сети глубина 0.*/
uint8_t  nwkMaxDepth = DEFAULT_MAX_DEPTH;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x85()
{
    nwkMaxDepth = DEFAULT_MAX_DEPTH;
}

REG_RESET( znet_reset_a0x85 );

