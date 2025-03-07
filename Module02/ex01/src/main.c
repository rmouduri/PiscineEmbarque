#include "ft_timer.h"
#include "ft_uart.h"


int main() {
    uart_init();
    timer0_init();

    for (;;);
}