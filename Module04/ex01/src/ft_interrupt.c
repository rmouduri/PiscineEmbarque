#include <avr/io.h>
#include <avr/interrupt.h>

#include "ft_interrupt.h"


inline static void init_timer1_interrupt(void) {
    // Clear OC1A/OC1B on compare match when up-counting.
    // Set OC1A/OC1B on compare match when down counting.
    TCCR1A |= (1 << COM1A1);

    // Phase and frequency correct mode, ICR1 TOP
    TCCR1B |= (1 << WGM13);
    
    // Prescaler 1
    TCCR1B |= (1 << CS10);

    // TOP
    ICR1 = 100;
    // Compare
    OCR1A = ICR1;

    // Reset counter
    TCNT1 = 0;
}

inline static void init_timer0_interrupt(void) {
    // Clear OC0A on compare match, set OC0A at BOTTOM, (non-inverting mode).
    TCCR0A |= (1 << COM0A1);

    // CTC
    TCCR0A |= (1 << WGM01);
    
    // Prescaler 64
    TCCR0B |= (1 << CS00) | (1 << CS01);

    // 16000000 / 200 (0 to 100, 100 to 0) / 64 = 1250, we select 250 and we will add a counter to 5 in ISR so 250 * 5 = 1250
    OCR0A = 250;

    // Enable compare interrupt
    TIMSK0 |= (1 << OCIE0A);

    // Reset counter
    TCNT0 = 0;
}

inline void init_interrupt(void) {
    init_timer1_interrupt();
    init_timer0_interrupt();

    sei();
}

ISR(TIMER0_COMPA_vect) {
    static int8_t order = -1, i = 0;

    if (++i >= 5) {
        OCR1A += order;
        i = 0;

        if (OCR1A == 0 || OCR1A == ICR1) {
            order *= -1;
        }
    }
}