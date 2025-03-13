#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ft_adc.h"
#include "ft_interrupt.h"
#include "ft_uart.h"


int main() {
    const char base[10] = "0123456789";
    int16_t temp;

    init_uart();
    init_adc(0, 1);

    for (;;) {
        // Read internal temperature sensor
        temp = read_adc(ADC_TEMP_SENSOR);
        temp = GET_TEMP(temp);

        uart_print_number_base(temp, "0123456789", sizeof(base));
        uart_printstr("\r\n");

        _delay_ms(20);
    }

    for (;;);
}