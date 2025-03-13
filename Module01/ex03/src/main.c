#include <avr/io.h>
#include <util/delay.h>

#include "btn_states.h"


uint8_t is_sw1_pressed(void) {
    static uint8_t sw2_state = BTN_RELEASED;

    if (sw2_state == BTN_RELEASED && !(PIND & (1 << PIND2))) {
        _delay_ms(20);

        if (!(PIND & (1 << PIND2))) {
            sw2_state = BTN_PRESSED;

            return SUCCESS;
        }
    } else if (sw2_state == BTN_PRESSED && (PIND & (1 << PIND2))) {
        _delay_ms(20);

        if (PIND & (1 << PIND2)) {
            sw2_state = BTN_RELEASED;
        }
    }

    return FAILURE;
}

uint8_t is_sw2_pressed(void) {
    static uint8_t sw2_state = BTN_RELEASED;

    if (sw2_state == BTN_RELEASED && !(PIND & (1 << PIND4))) {
        _delay_ms(20);

        if (!(PIND & (1 << PIND4))) {
            sw2_state = BTN_PRESSED;

            return SUCCESS;
        }
    } else if (sw2_state == BTN_PRESSED && (PIND & (1 << PIND4))) {
        _delay_ms(20);

        if (PIND & (1 << PIND4)) {
            sw2_state = BTN_RELEASED;
        }
    }

    return FAILURE;
}

int main() {
    // Set PB1 LED as receiver
    DDRB |= 1 << DD1;

    // Compare Output Mode, Fast PWM : Clear OC1 on compare, set OC1 at BOTTOM
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);

    // Set Waveform Generation Mode to CTC Mode (OCR1A becomes the TOP value to be reached) then
    // Set the clock with a prescale of 256 (256 clocks cycles increment TCNT0 by 1, we have 16M clock cycles per seconds)
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS12);

    // Set TOP value: 16M / 256 = 62500
    // Set TOP value: 16M / 256 = 62500 ; Loop in 1 sec
    ICR1 = 62500;

    // Set compare to 1/10 of ICR1 (1/10 of a sec)
    OCR1A = ICR1 / 10;

    // Reset counter
    TCNT0 = 0;

    for (;;) {
        if (is_sw1_pressed() == SUCCESS && OCR1A < ICR1) {
            OCR1A += ICR1/10;
        }

        if (is_sw2_pressed() == SUCCESS && OCR1A > (ICR1 / 10)) {
            OCR1A -= ICR1/10;
        }
    }
}