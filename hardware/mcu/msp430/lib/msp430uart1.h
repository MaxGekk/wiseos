#ifndef     _MSP430UART1_H
#define     _MSP430UART1_H

#include    <types.h>
#include    <msp430.h>

volatile uint8_t UCTL asm("0x0078");    /* UART Control */
volatile uint8_t UTCTL asm("0x0079");   /* UART Transmit Control */
volatile uint8_t URCTL asm("0x007A");   /* UART Receive Control */
volatile uint8_t UMCTL asm("0x007B");   /* UART Modulation Control */
volatile uint8_t UBR0 asm("0x007C");    /* UART Baud Rate 0 */
volatile uint8_t UBR1 asm("0x007D");    /* UART Baud Rate 1 */
volatile uint8_t URXBUF asm("0x007E");  /* UART Receive Buffer */
volatile uint8_t UTXBUF asm("0x007F");  /* UART Transmit Buffer */

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

#define UARTTX_IRQ      USART1TX_IRQ
#define UARTRX_IRQ      USART1RX_IRQ

#define MEx     ME2
#define IEx     IE2
#define IFGx    IFG2

#define USPIEx  USPIE1
#define UTXEx   UTXE1
#define URXEx   URXE1
#define UTXIFGx UTXIFG1
#define URXIFGx URXIFG1
#define UTXIEx  UTXIE1
#define URXIEx  URXIE1

#define UTXDx   6
#define URXDx   7

#endif                          /*  _MSP430UART1_H */
