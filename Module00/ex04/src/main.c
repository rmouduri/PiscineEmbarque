#include <avr/io.h>
#include <util/delay.h>

#include "btn_states.h"

struct struct_4bit_n
{
    uint8_t current: 4;
    uint8_t old: 4;
};


uint8_t is_sw1_pressed(void) {
    static uint8_t sw1_state = BTN_RELEASED, sw1_press_cnt = 0, sw1_release_cnt = 0;

    if (sw1_state == BTN_RELEASED && !(PIND & (1 << PIND2))) {
        _delay_ms(1);

        if (!(PIND & (1 << PIND2))) {
            ++sw1_press_cnt;
            if (sw1_press_cnt >= 20) {
                sw1_state = BTN_PRESSED;
                sw1_press_cnt = 0;

                return SUCCESS;
            }
        } else {
            sw1_press_cnt = 0;
        }
    } else if (sw1_state == BTN_PRESSED && (PIND & (1 << PIND2))) {
        _delay_ms(1);

        if (PIND & (1 << PIND2)) {
            ++sw1_release_cnt;
            if (sw1_release_cnt >= 20) {
                sw1_state = BTN_RELEASED;
                sw1_release_cnt = 0;
            }
        } else {
            sw1_release_cnt = 0;
        }
    }

    return FAILURE;
}

uint8_t is_sw2_pressed(void) {
    static uint8_t sw2_state = BTN_RELEASED, sw2_press_cnt = 0, sw2_release_cnt = 0;

    if (sw2_state == BTN_RELEASED && !(PIND & (1 << PIND4))) {
        _delay_ms(1);

        if (!(PIND & (1 << PIND4))) {
            ++sw2_press_cnt;
            if (sw2_press_cnt >= 20) {
                sw2_state = BTN_PRESSED;
                sw2_press_cnt = 0;

                return SUCCESS;
            }
        } else {
            sw2_press_cnt = 0;
        }
    }
    else if (sw2_state == BTN_PRESSED && (PIND & (1 << PIND4))) {
        _delay_ms(1);

        if (PIND & (1 << PIND4)) {
            ++sw2_release_cnt;
            if (sw2_release_cnt >= 20) {
                sw2_state = BTN_RELEASED;
                sw2_release_cnt = 0;
            }
        } else {
            sw2_release_cnt = 0;
        }
    }

    return FAILURE;
}

inline void turn_on_leds(const uint8_t n) {
    // Set all LEDs Bits to 0 then set individual bits to correct value
    PORTB = (PORTB & ~0b10111) | (n & 0b111) | ((n & 0b1000) << 1);
}

int main() {
    struct struct_4bit_n n = { 0 };

    DDRB |= 1 << DD0;
    DDRB |= 1 << DD1;
    DDRB |= 1 << DD2;
    DDRB |= 1 << DD4;

    for (;;) {
        // Read SW1 Button
        if (is_sw1_pressed() == SUCCESS) {
            n.old = n.current;
            n.current = n.current == 15 ? 0 : n.current + 1;
        }

        // Read SW2 Button
        if (is_sw2_pressed() == SUCCESS) {
            n.old = n.current;
            n.current = n.current == 0 ? 15 : n.current - 1;
        }

        // Turn ON LEDs
        if (n.current != n.old) {
            turn_on_leds(n.current);
        }
    }
}