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

inline static void display_7segment_digit(const uint8_t n, const uint8_t index) {
    const uint8_t segm7_tab[10] = SEGM7_TAB;
    const uint8_t segm7_can[4]  = SEGM7_CAN;

    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
    i2c_write(OUTPUT_PORT0);
    i2c_write(~segm7_can[index]);
    i2c_stop();

    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
    i2c_write(OUTPUT_PORT1);
    i2c_write(segm7_tab[n]);
    i2c_stop();
}

inline void turn_on_7segment(const uint16_t n, const uint8_t display_left_numbers) {
    uint8_t digits[4] = {2, 0, 0, 0};

    if (n >= 1000 || display_left_numbers) {
        display_7segment_digit(digits[3], 3);
    }

    if (n >= 100 || display_left_numbers) {
        display_7segment_digit(digits[2], 2);
    }

    if (n >= 10 || display_left_numbers) {
        display_7segment_digit(digits[1], 1);
    }

    display_7segment_digit(digits[0], 0);
}