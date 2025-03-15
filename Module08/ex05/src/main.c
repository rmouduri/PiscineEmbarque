#include <util/delay.h>

#include "ft_adc.h"
#include "ft_spi.h"


# define BTN_RELEASED 0
# define BTN_PRESSED 1

# define SUCCESS 0
# define FAILURE 1

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

inline static void set_led(led_frame_t *led_frame, const uint8_t color_index, const uint8_t val) {
    if (color_index == 0) {
        led_frame->red = val;
    } else if (color_index == 1) {
        led_frame->green = val;
    } else if (color_index == 2) {
        led_frame->blue = val;
    }
}

int main() {
    uint8_t led_index = 0;
    uint8_t color_index = 0;
    uint8_t saved_color = 0;
    led_frame_t leds[3] = {
        get_led_frame(0x0, 0x0, 0x0),
        get_led_frame(0x0, 0x0, 0x0),
        get_led_frame(0x0, 0x0, 0x0)
    };

    init_spi();
    init_adc(1, 0);

    for (;;) {
        const uint8_t pot = read_adc(ADC_POT);
        
        if (is_sw1_pressed() == SUCCESS) {
            color_index = (color_index + 1) % 3;
            saved_color = color_index == 0 ? (leds[led_index].red) : (color_index == 1 ? leds[led_index].green : leds[led_index].blue);
        }

        if (is_sw2_pressed() == SUCCESS) {
            set_led(&leds[led_index], color_index, saved_color);
            led_index = (led_index + 1) % 4;
            color_index = 0;
        }

        if (led_index != 3) {
            set_led(&leds[led_index], color_index, pot);
        }

        set_rgb_spi(leds[0], leds[1], leds[2]);
    }
}