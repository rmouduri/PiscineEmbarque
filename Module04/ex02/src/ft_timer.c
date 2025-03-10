#include <avr/io.h>
#include <avr/interrupt.h>

#include "ft_timer.h"
#include "ft_uart.h"


inline void timer0_init(void) {
    // Enable global interrupts
    sei();

    // Set Waveform Generation Mode to CTC Mode (OCR1A becomes the TOP value to be reached) then
    // Set the clock with a prescale of 1024 (1024 clocks cycles increment TCNT0 by 1, we have 16M clock cycles per seconds)
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

    // Set TOP value: 16M / 1024 * 2 = 31250 ; 1 toggles every 2 sec
    OCR1A = 31250;

    // Enable compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Reset counter
    TCNT1 = 0;
}