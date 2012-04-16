#ifndef     _IRQ_H
#define     _IRQ_H

#include    <types.h>

typedef result_t    (*irq_handler_t)();

#define IRQ_HANDLER( irq_num ) _IRQ_HANDLER_( irq_num )
#define _IRQ_HANDLER_( irq_num ) __attribute__((used)) __##irq_num##_interrupt()

void    process_irq( uint16_t irq_num );

#endif  /*  _IRQ_H */

