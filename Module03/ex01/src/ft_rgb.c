#include <avr/io.h>

#include "ft_rgb.h"


inline void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    r = r ? 1 : 0;
    g = g ? 1 : 0;
    b = b ? 1 : 0;

    PORTD = (PORTD & ~((1 << LED_D5_RED) | (1 << LED_D5_GREEN) | (1 << LED_D5_BLUE)))
        | (r << LED_D5_RED) | (g << LED_D5_GREEN) | (b << LED_D5_BLUE);
}