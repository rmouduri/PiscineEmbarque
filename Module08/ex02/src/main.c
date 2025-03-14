#include <stdint.h>
#include <util/delay.h>

#include "ft_spi.h"


int main() {
    init_spi();
    
    for (;;) {
        set_rgb_spi(
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY)
        );

        _delay_ms(250);

        set_rgb_spi(
            get_led_frame(0xff, 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY)
        );

        _delay_ms(250);

        set_rgb_spi(
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0xff, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY)
        );

        _delay_ms(250);

        set_rgb_spi(
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0xff, LED_INTENSITY)
        );

        _delay_ms(250);
    }
}