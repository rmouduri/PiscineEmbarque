#include <avr/io.h>


int main() {
    uint32_t n;

    DDRB |= 1 << DD1;

    for (;;) {
        n = 0;
        // Au pif :-)
        while (++n < 1350000);

        PORTB ^= 1 << PORTB1;
    }
}