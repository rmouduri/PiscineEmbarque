#include <avr/io.h>
#include <util/delay.h>

#include "ft_rgb.h"


int main() {
    const uint8_t   color_array[7][3] = COLOR_ARRAY;
    uint8_t         i = 0;

    DDRD |= (1 << LED_D5_RED) | (1 << LED_D5_GREEN) | (1 << LED_D5_BLUE);

    for (;;) {
        set_rgb(color_array[i][0], color_array[i][1], color_array[i][2]);
        _delay_ms(1000);

        i = (i + 1) % 7;
    }
}