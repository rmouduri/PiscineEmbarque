#include <stdint.h>
#include <util/delay.h>

#include "ft_spi.h"

# define COLOR_ARRAY    { \
    /* Red */       {0xff, 0, 0}, \
    /* Green */     {0, 0xff, 0}, \
    /* Blue */      {0, 0, 0xff}, \
    /* Yellow */    {0xff, 0xff, 0}, \
    /* Cyan */      {0, 0xff, 0xff}, \
    /* Magenta */   {0xff, 0, 0xff}, \
    /* White */     {0xff, 0xff, 0xff}, \
}

int main() {
    const uint8_t   color_array[7][3] = COLOR_ARRAY;
    uint8_t i = 0;

    init_spi();

    
    for (;;) {
        set_rgb_spi(
            get_led_frame(color_array[i][0], color_array[i][1], color_array[i][2], LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, 0x0, LED_INTENSITY)
        );

        _delay_ms(1000);

        i = (i + 1) % 7;
    }
}