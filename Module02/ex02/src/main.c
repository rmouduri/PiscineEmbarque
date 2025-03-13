#include "ft_uart.h"


int main() {
    uart_init();

    for (;;) {
        uart_tx(uart_rx());
    }
}