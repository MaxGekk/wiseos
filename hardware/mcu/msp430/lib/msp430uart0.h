#ifndef     _MSP430UART0_H
#define     _MSP430UART0_H

#include    <types.h>
#include    <msp430.h>

volatile uint8_t UCTL asm("0x0070");    /* UART Control */
volatile uint8_t UTCTL asm("0x0071");   /* UART Transmit Control */
volatile uint8_t URCTL asm("0x0072");   /* UART Receive Control */
volatile uint8_t UMCTL asm("0x0073");   /* UART Modulation Control */
volatile uint8_t UBR0 asm("0x0074");    /* UART Baud Rate 0 */
volatile uint8_t UBR1 asm("0x0075");    /* UART Baud Rate 1 */
volatile uint8_t URXBUF asm("0x0076");  /* UART Receive Buffer */
volatile uint8_t UTXBUF asm("0x0077");  /* UART Transmit Buffer */

#define PENA                0x80    /* UCTL */
#define PEV                 0x40
#define SPB                 0x20    /* to distinguish from stackpointer SP */
#define CHAR                0x10
#define LISTEN              0x08    /* Loopback */
#define SYNC                0x04    /* USART synchronous/asynchronous */
#define MM                  0x02
#define SWRST               0x01

#define CKPH                0x80    /* UTCTL */
#define CKPL                0x40
#define SSEL1               0x20
#define SSEL0               0x10
#define URXSE               0x08
#define TXWAKE              0x04
#define STC                 0x02
#define TXEPT               0x01

#define FE                  0x80    /* URCTL */
#define PE                  0x40
#define OE                  0x20
#define BRK                 0x10
#define URXEIE              0x08
#define URXWIE              0x04
#define RXWAKE              0x02
#define RXERR               0x01

#define SSEL_0              0x00    /* UCLKI */
#define SSEL_1              0x10    /* ACLK  */
#define SSEL_2              0x20    /* SMCLK */
#define SSEL_3              0x30    /* SMCLK */

#define SSEL_UCLKI          SSEL_0
#define SSEL_ACLK           SSEL_1
#define SSEL_SMCLK          SSEL_2

#define UARTTX_IRQ      USART0TX_IRQ
#define UARTRX_IRQ      USART0RX_IRQ

#define MEx     ME1
#define IEx     IE1
#define IFGx    IFG1

#define USPIEx  USPIE0
#define UTXEx   UTXE0
#define URXEx   URXE0
#define UTXIFGx UTXIFG0
#define URXIFGx URXIFG0
#define UTXIEx  UTXIE0
#define URXIEx  URXIE0

#define UTXDx   4
#define URXDx   5

#endif                          /*  _MSP430UART0_H */
