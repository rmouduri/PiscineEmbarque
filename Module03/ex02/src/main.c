#include <avr/io.h>
#include <util/delay.h>

#include "ft_rgb.h"


int main() {
    uint8_t i = 0;

    init_rgb();

    for (;;) {
        wheel(i++);
        _delay_ms(10);
    }
}