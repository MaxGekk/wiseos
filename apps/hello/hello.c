#include <init.h>
#include <task.h>
#include <debug.h>

uint32_t seq_num = 0;

void print_hello();
TASK( print_hello, 9 );

void print_hello()
{
    DBG1("Hello, World! [ %ld ]", seq_num );
    seq_num += 1;
    POST_TASK( print_hello );
}

void start_hello()
{
    POST_TASK( print_hello );
}
INIT( start_hello );

