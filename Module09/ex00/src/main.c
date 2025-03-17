#include <avr/io.h>
#include <util/delay.h>

#include "ft_expander.h"
#include "ft_i2c.h"
#include "ft_uart.h"

int main() {
    init_uart();
    i2c_init();

    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
    i2c_write(CONFIG_PORT0);
    i2c_write(~PORT0_D09);
    i2c_stop();

    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
    i2c_write(OUTPUT_PORT0);
    i2c_write(~PORT0_D09);
    i2c_stop();

    for (;;);
}