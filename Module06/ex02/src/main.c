#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#include "ft_i2c.h"
#include "ft_uart.h"

int main() {
    _delay_ms(40); // For AHT20

    init_uart();

    i2c_init();
    i2c_start();
    i2c_write(TWDR);
    i2c_stop();

    _delay_ms(10); // After initializing AHT20

    for (;;) {
        read_temp_humidity();
    }
}