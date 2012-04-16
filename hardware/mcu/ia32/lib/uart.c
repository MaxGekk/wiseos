#include    <task.h>
#include    <uart.h>
#include    <stdio.h>

void    uart_start() 
{
    return;
}

void    tx_done_notify()
{
    uart_tx_done();
}
TASK( tx_done_notify, 9 );

void    uart_tx( const uint8_t  data )
{
    printf("%hhx ",data);
    fflush(stdout);
    POST_TASK( tx_done_notify );
}

