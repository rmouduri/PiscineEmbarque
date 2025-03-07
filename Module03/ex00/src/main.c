#include <avr/io.h>
#include <util/delay.h>

#include "ft_rgb.h"


int main() {
    DDRD |= (1 << LED_D5_RED) | (1 << LED_D5_GREEN) | (1 << LED_D5_BLUE);

    for (;;) {
        set_rgb(255, 0, 0);
        _delay_ms(1000);

        set_rgb(0, 255, 0);
        _delay_ms(1000);

        set_rgb(0, 0, 255);
        _delay_ms(1000);
    }
}