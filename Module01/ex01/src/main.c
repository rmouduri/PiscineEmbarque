#include <avr/io.h>


int main() {
    // Set PB1 LED as receiver
    DDRB |= 1 << DD1;

    // Toggle OC1A (PB1) when TCNT0 reaches TOP value (that will be defined in OCR1A later)
    TCCR1A |= (1 << COM1A0);

    // Set Waveform Generation Mode to CTC Mode (OCR1A becomes the TOP value to be reached) then
    // Set the clock with a prescale of 256 (256 clocks cycles increment TCNT0 by 1, we have 16M clock cycles per seconds)
    TCCR1B |= (1 << WGM12) | (1 << CS12);

    // Set TOP value: 16M / 256 / 2 = 31250 ; 2 toggles per, turn ON and OFF OC1A (PB1 LED) in 1 sec
    OCR1A = 31250;

    // Reset counter
    TCNT0 = 0;

    for (;;);
}