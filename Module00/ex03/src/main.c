#include <avr/io.h>
#include <util/delay.h>

#include "btn_states.h"


int main() {
    uint8_t btn_state = BTN_RELEASED;
    uint8_t press_cnt = 0;
    uint8_t release_cnt = 0;
    
    DDRB |= 1 << DD0;

    for (;;) {
        // Read SW1 Button
        if (btn_state == BTN_RELEASED && !(PIND & (1 << PIND2))) {
            _delay_ms(1);

            if (!(PIND & (1 << PIND2))) {
                ++press_cnt;
                if (press_cnt >= 20) {
                    PORTB ^= 1 << PORTB0;
                    btn_state = BTN_PRESSED;
                    press_cnt = 0;
                }
            } else {
                press_cnt = 0;
            }
        }
        else if (btn_state == BTN_PRESSED && (PIND & (1 << PIND2))) {
            _delay_ms(1);

            if (PIND & (1 << PIND2)) {
                ++release_cnt;
                if (release_cnt >= 20) {
                    btn_state = BTN_RELEASED;
                    release_cnt = 0;
                }
            } else {
                release_cnt = 0;
            }
        }
    }
}