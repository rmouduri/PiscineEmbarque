#include <avr/io.h>
#include <avr/interrupt.h>

#include "ft_interrupt.h"


inline static void init_timer1_interrupt(void) {
    // Prescaler 256 + CTC Mode
    TCCR1B |= (1 << CS12) | (1 << WGM12);

    // We want every 20ms, so 16000000 / 50 / PRESCALER (256)
    OCR1A = 1250;
    // OCR1B = 1250;

    // Enable compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    // TIMSK1 |= (1 << OCIE1B);

    // Reset counter
    TCNT1 = 0;
}

inline void init_interrupt(void) {
    init_timer1_interrupt();

    // Activate interruptions on PORTD
    PCICR |= (1 << PCIE2);

    // Mask to activate interruption only from PD2
    PCMSK2 |= (1 << PCINT18);
    // PCMSK2 |= (1 << PCINT20);
    
    sei();
}


volatile uint8_t sw1_rdy = 0;
// volatile uint8_t sw2_rdy = 0;

ISR(PCINT2_vect) {
    cli();

    extern volatile uint8_t sw1_rdy;
    static uint8_t sw1_state = 1;

    if (sw1_rdy) {
        if (sw1_state) {
            PORTB ^= 1 << PORTB0;
        }

        sw1_state = !sw1_state;
        sw1_rdy = 0;
    }

    TCNT1 = 0;
    sei();
}

ISR(TIMER1_COMPA_vect) {
    cli();

    extern volatile uint8_t sw1_rdy;

    sw1_rdy = 1;

    sei();
}

// ISR(TIMER1_COMPB_vect) {
//     cli();

//     extern volatile uint8_t sw2_rdy;

//     sw2_rdy = 1;

//     sei();
// }