#include    <critical.h>
#include    <irq.h>
#include    <msp430.h>
#include    "msp430uart1.h"

IRQ_HANDLER( UARTRX_IRQ )
{
    uint8_t temp = URXBUF;
    uart_rx_done(temp);
}

IRQ_HANDLER( UARTTX_IRQ )
{
    /* is i2c ? */
    if ((UCTL & 0x20) && (UCTL & 0x04) && (UCTL & 0x01))
        return;
    uart_tx_done();
}

void uart_tx(const uint8_t data)
{
    UTXBUF = data;
}

#define     UMCTL_SMCLK_115200  0x10

void uart_start()
{
    critical_enter();

    UCTL = SWRST;
    UCTL |= CHAR;

    URCTL &= ~URXEIE;           // even erroneous characters trigger interrupts

    UCTL = SWRST;
    UCTL |= CHAR;               // 8-bit char, UART-mode

    UTCTL &= ~(SSEL_0 | SSEL_1 | SSEL_2 | SSEL_3);
    UTCTL |= SSEL_SMCLK;

    UBR0 = 70 & 0x0FF;
    UBR1 = (70 >> 8) & 0x0FF;
    UMCTL = UMCTL_SMCLK_115200;

    MEx &= ~USPIEx;             // USART0 SPI module disable
    MEx |= (UTXEx | URXEx);     //USART0 UART module enable;

    UCTL &= ~SWRST;

    IFGx &= ~(UTXIFGx | URXIFGx);
    IEx |= (UTXIEx | URXIEx);   // interrupt enable

    P3SEL |= (1 << UTXDx);
    P3DIR |= (1 << UTXDx);
    P3SEL |= (1 << URXDx);
    P3DIR |= (1 << URXDx);

    critical_exit();
}

void uart_stop()
{
    critical_enter();

    IFGx &= ~(UTXIFGx | URXIFGx);
    IEx &= ~(UTXIEx | URXIEx);  // interrupt disabled

    MEx &= ~(UTXEx | URXEx);    // USART0 UART module disable

    P3SEL &= ~(1 << UTXDx);
    P3SEL &= ~(1 << URXDx);

    critical_exit();
}
