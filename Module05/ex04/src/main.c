#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ft_adc.h"
#include "ft_interrupt.h"
#include "ft_rgb.h"


int main() {
    uint8_t pot;

    init_rgb();
    init_adc(1, 0);

    // Set the D1, D2, D3 and D4 LEDs as receiver
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

    for (;;) {
        // Read internal temperature sensor
        pot = read_adc(ADC_POT);

        // Turn ON D1, D2, D3 and D4 LEDs
        PORTB = (PORTB & ~(1 << PORTB0)) | ((pot >= UINT8_MAX * .25) << PORTB0);
        PORTB = (PORTB & ~(1 << PORTB1)) | ((pot >= UINT8_MAX * .50) << PORTB1);
        PORTB = (PORTB & ~(1 << PORTB2)) | ((pot >= UINT8_MAX * .75) << PORTB2);
        PORTB = (PORTB & ~(1 << PORTB4)) | ((pot >= UINT8_MAX) << PORTB4);

        // Turn ON RGB LED
        wheel(pot);

        _delay_ms(20);
    }
}