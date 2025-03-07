#include <avr/io.h>

int main() {
    DDRB |= 1 << DD0;

    for (;;) {
        // Read SW1 Button
        if (PIND & (1 << PIND2)) {
            PORTB &= ~(1 << PORTB0);
        } else {
            PORTB |= 1 << PORTB0;
        }
    }
}
