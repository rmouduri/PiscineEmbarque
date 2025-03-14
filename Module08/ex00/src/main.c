#include <stdint.h>

#include "ft_spi.h"

int main() {
    init_spi();

    set_rgb_spi(
        get_led_frame(0xff, 0x0, 0x0, LED_INTENSITY),
        get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY),
        get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY)
    );

    for (;;);
}