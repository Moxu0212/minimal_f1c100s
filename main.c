#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern void sys_uart_putc(char c);

int boot_main(int argc, char **argv) {
    
    while(1)
    {
        sys_uart_putc('A');
    }
    return 0;
}
