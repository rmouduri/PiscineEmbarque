#include <avr/io.h>
#include <util/twi.h>

#include "ft_i2c.h"
#include "ft_uart.h"

int main() {
    init_uart();

    i2c_init();
    i2c_start();
    i2c_stop();

    for (;;);
}