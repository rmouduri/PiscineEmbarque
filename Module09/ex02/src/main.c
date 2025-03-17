#include <avr/io.h>
#include <util/delay.h>

#include "ft_expander.h"
#include "ft_i2c.h"


int main() {
    i2c_init();

    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
    i2c_write(CONFIG_PORT0);
    i2c_write((uint8_t) ~(PORT0_CA1 | PORT0_CA2 | PORT0_CA3 | PORT0_CA4));
    i2c_stop();

    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
    i2c_write(CONFIG_PORT1);
    i2c_write(0);
    i2c_stop();

    turn_on_7segment(2, 0);

    for (;;);
}