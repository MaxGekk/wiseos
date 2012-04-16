#include <init.h>
#include <mcu.h>
#include <msp430.h>

#define BASE_CLK_8MHZ

void irq_enable()
{
    asm("eint"::);
}

void irq_disable()
{
    asm("dint"::);
    asm("nop"::);
}

void system_sleep()
{
}

static inline void init_clocks()
{
#ifdef BASE_CLK_8MHZ
    uint8_t i;
    BCSCTL1 = (RSEL2 | RSEL1 | RSEL0);
    DCOCTL = 0;
    do {
        CLR_FLAG(IFG1, OFIFG);
        i = 0xff;
        while (--i) ;
    } while (READ_FLAG(IFG1, OFIFG));
    BCSCTL2 = SELM1 | SELS;
    CLR_FLAG(IE1, OFIE);
#else
    DCOCTL = (DCO1);
    BCSCTL1 = XT2OFF | (RSEL2 | RSEL1 | RSEL0);
    BCSCTL2 = DCOR;
    //BCSCTL2 = 0;
    CLR_FLAG(IE1, OFIE);
#endif
}

void init_msp430()
{
    init_clocks();
}

INIT(init_msp430);
