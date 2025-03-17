#include <stdint.h>

#include "ft_expander.h"
#include "ft_i2c.h"


inline uint8_t get_sw3(void) {
    uint8_t port0;

    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
    i2c_write(INPUT_PORT0);
    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_READ_MODE);
    port0 = i2c_read(0);
    i2c_stop();

    return (port0 & PORT0_SW3);
}