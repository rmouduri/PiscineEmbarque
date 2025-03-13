#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ft_adc.h"
#include "ft_interrupt.h"
#include "ft_uart.h"


int main() {
    const char base[16] = "0123456789abcdef";
    uint8_t result;

    init_uart();
    init_adc(1);

    for (;;) {
        // Read potentiometer
        result = read_adc(ADC_POT);
        if (result < sizeof(base)) {
            uart_tx('0');
        }
        uart_print_number_base(result, base, sizeof(base));
        uart_printstr(", ");

        // Read LDR
        result = read_adc(ADC_LDR);
        if (result < sizeof(base)) {
            uart_tx('0');
        }
        uart_print_number_base(result, base, sizeof(base));
        uart_printstr(", ");

        // Read NTC
        result = read_adc(ADC_NTC);
        if (result < sizeof(base)) {
            uart_tx('0');
        }
        uart_print_number_base(result, base, sizeof(base));
        uart_printstr("\r\n");

        _delay_ms(20);
    }

    for (;;);
}