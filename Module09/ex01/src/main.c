#include <avr/io.h>
#include <util/delay.h>

#include "btn_states.h"
#include "ft_expander.h"
#include "ft_i2c.h"


inline static uint8_t is_sw3_pressed(void) {
    static uint8_t sw3_state = BTN_RELEASED;

    if (sw3_state == BTN_RELEASED && !(get_sw3())) {
        _delay_ms(20);

        if (!(get_sw3())) {
            sw3_state = BTN_PRESSED;

            return SUCCESS;
        }
    } else if (sw3_state == BTN_PRESSED && (get_sw3())) {
        _delay_ms(20);

        if (get_sw3()) {
            sw3_state = BTN_RELEASED;
        }
    }

    return FAILURE;
}

int main() {
    uint8_t i = 0;

    i2c_init();

    i2c_start();
    i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
    i2c_write(CONFIG_PORT0);
    i2c_write(~(PORT0_D09 | PORT0_D10 | PORT0_D11));
    i2c_stop();

    for (;;) {
        if (is_sw3_pressed() == SUCCESS) {
            i = (i + 1) % 8;
        }

        i2c_start();
        i2c_slave_connect(EXPANDER_ADDRESS, I2C_WRITE_MODE);
        i2c_write(OUTPUT_PORT0);
        i2c_write(~(((i & 0b100) ? PORT0_D09 : 0) | ((i & 0b010) ? PORT0_D10 : 0) | ((i & 0b1) ? PORT0_D11 : 0)));
        i2c_stop();
    }
}