#include <avr/io.h>

#include "ft_interrupt.h"


inline void init_timer1_interrupt(void) {
    // Clear OC1A/OC1B on compare match when up-counting.
    // Set OC1A/OC1B on compare match when down counting.
    TCCR1A |= (1 << COM1A1) | (1 << WGM10) | (1 << WGM11);

    // Fast PWN, Top OCR1, TOP1 set at TOP
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    
    // Prescaler 256
    TCCR1B |= (1 << CS12);

    // 16000000 / 256 = 62500, will go in ISR every second
    OCR1A = 62500;

    // Enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);

    // Reset counter
    TCNT1 = 0;

    __asm__ __volatile__ ("sei" ::: "memory");
}