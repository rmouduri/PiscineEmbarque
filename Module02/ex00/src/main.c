#include <avr/io.h>
#include <util/delay.h>

#include "ft_uart.h"


int main() {
    uart_init();

    for (;;) {
        uart_tx('Z');

        _delay_ms(1000);
    }
}