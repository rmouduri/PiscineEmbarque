#include <avr/io.h>
#include <util/delay.h>

#include "ft_uart.h"

int main() {
    uint8_t i = 0;

    uart_init();
    init_prompt();

    // Set the D1, D2, D3 and D4 LEDs as receiver
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

    for (;;) {
        if (state == CONNECTED_MODE) {
            ++i;

            PORTB = (PORTB & ~(1 << PORTB0)) | ((i % 15 ? 0 : 1) << PORTB0);
            PORTB = (PORTB & ~(1 << PORTB1)) | ((i % 7 ? 0 : 1) << PORTB1);
            PORTB = (PORTB & ~(1 << PORTB2)) | ((i % 11 ? 0 : 1) << PORTB2);
            PORTB = (PORTB & ~(1 << PORTB4)) | ((i % 22 ? 0 : 1) << PORTB4);

            _delay_ms(10);
        }
    }
}