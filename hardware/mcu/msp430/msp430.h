#ifndef     _MSP430_H
#define     _MSP430_H

#include    <types.h>

#define SET_FLAG(port, flag) ((port) |= (flag))
#define CLR_FLAG(port, flag) ((port) &= ~(flag))
#define READ_FLAG(port, flag) ((port) & (flag))

#define DACDMA_IRQ       0   /* 0xFFE0 DAC12/DMA */
#define PORT2_IRQ        1   /* 0xFFE2 Port 2 */
#define USART1TX_IRQ     2   /* 0xFFF4 USART 1 Transmit */
#define USART1RX_IRQ     3   /* 0xFFF6 USART 1 Receive */
#define PORT1_IRQ        4   /* 0xFFE8 Port 1 */
#define TIMERA1_IRQ      5  /* 0xFFEA Timer A CC1-2, TA */
#define TIMERA0_IRQ      6  /* 0xFFEC Timer A CC0 */
#define ADC12_IRQ        7  /* 0xFFEE ADC */
#define USART0TX_IRQ     8  /* 0xFFF0 USART 0 Transmit */
#define USART0RX_IRQ     9  /* 0xFFF2 USART 0 Receive, I2C tx/rx */
#define WDT_IRQ          10  /* 0xFFF4 Watchdog Timer */
#define COMPARATORA_IRQ  11  /* 0xFFF6 Comparator A */
#define TIMERB1_IRQ      12  /* 0xFFF8 Timer B CC1-2, TB */
#define TIMERB0_IRQ      13  /* 0xFFFA Timer B CC0 */
#define NMI_IRQ          14  /* 0xFFFC Non-maskable */

volatile uint8_t IE1 asm("0x0000");
#define WDTIE               (1<<0)
#define OFIE                (1<<1)
#define NMIIE               (1<<4)
#define ACCVIE              (1<<5)
#define URXIE0              (1<<6)
#define UTXIE0              (1<<7)

volatile uint8_t IE2 asm("0x0001"); /* Interrupt Enable 2 */
#define URXIE1              (1<<4)
#define UTXIE1              (1<<5)

volatile uint8_t IFG1 asm("0x0002");
#define WDTIFG              (1<<0)
#define OFIFG               (1<<1)
#define NMIIFG              (1<<4)
#define URXIFG0             (1<<6)
#define UTXIFG0             (1<<7)

volatile uint8_t IFG2 asm("0x0003");    /* Interrupt Flag 2 */
#define URXIFG1             (1<<4)
#define UTXIFG1             (1<<5)

volatile uint8_t ME1 asm("0x0004"); /* Module Enable 1 */
#define URXE0               (1<<6)
#define USPIE0              (1<<6)
#define UTXE0               (1<<7)

volatile uint8_t ME2 asm("0x0005"); /* Module Enable 2 */
#define URXE1               (1<<4)
#define USPIE1              (1<<4)
#define UTXE1               (1<<5)

/* clock */
volatile uint8_t DCOCTL asm("0x0056");
volatile uint8_t BCSCTL1 asm("0x0057");
volatile uint8_t BCSCTL2 asm("0x0058");

#define DCO0                0x20    /* DCO Select Bit 0 */
#define DCO1                0x40    /* DCO Select Bit 1 */
#define DCO2                0x80    /* DCO Select Bit 2 */

#define RSEL0               0x01    /* Resistor Select Bit 0 */
#define RSEL1               0x02    /* Resistor Select Bit 1 */
#define RSEL2               0x04    /* Resistor Select Bit 2 */

#define SELS                0x08    /* SMCLK Source Select 0:DCOCLK / 1:XT2CLK/LFXTCLK */
#define SELM0               0x40    /* MCLK Source Select 0 */
#define SELM1               0x80    /* MCLK Source Select 1 */

/* gpio */
volatile uint8_t P3IN asm("0x0018");    /* Port 3 Input */
volatile uint8_t P3OUT asm("0x0019");   /* Port 3 Output */
volatile uint8_t P3DIR asm("0x001A");   /* Port 3 Direction */
volatile uint8_t P3SEL asm("0x001B");   /* Port 3 Selection */

#endif                          /* _MSP430_H */
