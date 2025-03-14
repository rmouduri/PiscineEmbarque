#include <avr/io.h>

#include "ft_rgb.h"


inline void init_rgb(void) {
    // Set the R, G and B LEDs as receiver
    DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD3);


    // Set PWM Phase Correct mode so TOP is 0xFF
    TCCR0A |= (1 << WGM00);

    // Clear OC0B (Red LED) on compare match, set OC0B (Red LED) at BOTTOM
    TCCR0A |= (1 << COM0B1);
    // Clear OC0A (Green LED) on compare match, set OC0A (Green LED) at BOTTOM
    TCCR0A |= (1 << COM0A1);

    // Set TCCR0B prescaler to 1: Table 14-9. Clock Select Bit Description
    TCCR0B |= (1 << CS00);


    // Set PWM Phase Correct mode so TOP is 0xFF
    TCCR2A |= (1 << WGM20);

    // Clear OC0B (Blue LED) on compare match, set OC0B (Blue LED) at BOTTOM
    TCCR2A |= (1 << COM2B1);

    // Set TCCR2B prescaler to 1: Table 17-9. Clock Select Bit Description
    TCCR2B |= (1 << CS20);
}

inline void set_rgb(const uint8_t r, const uint8_t g, const uint8_t b) {
    OCR0B = r;
    OCR0A = g;
    OCR2B = b;
}

inline void wheel(uint8_t pos) {
    pos = 255 - pos;

    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}