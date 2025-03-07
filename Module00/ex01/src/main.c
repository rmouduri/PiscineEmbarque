#include <avr/io.h>

int main() {
    // Turn on the D1 LED linked to PORTB0 Register
    // cf. Back of the board, subject and chapter 13.4.1
    PORTB |= 1 << PORTB0;

    // Brighter D0 LED
    // cf. Chapter 13.2.3
    DDRB |= 1 << DDB0;

    for (;;);
}