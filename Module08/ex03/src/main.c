#include <stdint.h>

#include "ft_adc.h"
#include "ft_spi.h"


int main() {
    init_adc(1, 0);
    init_spi();
    
    for (;;) {
        const uint8_t pot = read_adc(ADC_POT);

        set_rgb_spi(
            get_led_frame((pot >= (UINT8_MAX * .33) ? 0xff : 0x0), 0x0, 0x0, LED_INTENSITY),
            get_led_frame(0x0, (pot >= (UINT8_MAX * .66) ? 0xff : 0x0), 0x0, LED_INTENSITY),
            get_led_frame(0x0, 0x0, (pot >= UINT8_MAX ? 0xff : 0x0), LED_INTENSITY)
        );
    }
}