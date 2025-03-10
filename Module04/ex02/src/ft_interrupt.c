#include <avr/io.h>
#include <avr/interrupt.h>

#include "ft_interrupt.h"
#include "btn_states.h"


inline static void init_timer1_interrupt(void) {
    // Prescaler 256 + CTC Mode
    TCCR1B |= (1 << CS12) | (1 << WGM12);

    // We want every 20ms, so 16000000 / 50 / PRESCALER (256)
    OCR1A = 1250;
    OCR1B = 1250;

    // Enable compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    TIMSK1 |= (1 << OCIE1B);

    // Reset counter
    TCNT1 = 0;
}

inline void init_interrupt(void) {
    init_timer1_interrupt();

    // Activate interruptions on PORTD
    PCICR |= (1 << PCIE2);

    // Mask to activate interruption only from PD2 and PD4
    PCMSK2 |= (1 << PCINT18);
    PCMSK2 |= (1 << PCINT20);
    
    sei();
}

inline void turn_on_leds(const uint8_t n) {
    // Set all LEDs Bits to 0 then set individual bits to correct value
    PORTB = (PORTB & 0b01000) | (n & 0b111) | ((n & 0b1000) << 1);
}


volatile uint8_t sw1_rdy = 0;
volatile uint8_t sw2_rdy = 0;

uint8_t is_interrupt_sw1_pressed(void) {
    static uint8_t sw1_state = BTN_RELEASED;
    extern volatile uint8_t sw1_rdy;

    if (sw1_rdy && sw1_state == BTN_RELEASED && !(PIND & (1 << PIND2))) {

        if (!(PIND & (1 << PIND2))) {
            sw1_state = BTN_PRESSED;

            return SUCCESS;
        }
    } else if (sw1_rdy && sw1_state == BTN_PRESSED && (PIND & (1 << PIND2))) {

        if (PIND & (1 << PIND2)) {
            sw1_state = BTN_RELEASED;
        }
    }

    return FAILURE;
}

uint8_t is_interrupt_sw2_pressed(void) {
    static uint8_t sw2_state = BTN_RELEASED;
    extern volatile uint8_t sw2_rdy;

    if (sw2_rdy && sw2_state == BTN_RELEASED && !(PIND & (1 << PIND4))) {

        if (!(PIND & (1 << PIND4))) {
            sw2_state = BTN_PRESSED;

            return SUCCESS;
        }
    } else if (sw2_rdy && sw2_state == BTN_PRESSED && (PIND & (1 << PIND4))) {

        if (PIND & (1 << PIND4)) {
            sw2_state = BTN_RELEASED;
        }
    }

    return FAILURE;
}

ISR(PCINT2_vect) {
    extern volatile uint8_t sw1_rdy, sw2_rdy;
    static uint8_t n = 0;


    if (is_interrupt_sw1_pressed() == SUCCESS) {
        n = n == 15 ? 0 : n + 1;
        turn_on_leds(n);
        sw1_rdy = 0;
    }

    if (is_interrupt_sw2_pressed() == SUCCESS) {
        n = n == 0 ? 15 : n - 1;
        turn_on_leds(n);
        sw2_rdy = 0;
    }

    TCNT1 = 0;
}

ISR(TIMER1_COMPA_vect) {
    extern volatile uint8_t sw1_rdy;

    sw1_rdy = 1;
}

ISR(TIMER1_COMPB_vect) {
    extern volatile uint8_t sw2_rdy;

    sw2_rdy = 1;
}