#include <stdint.h>
#include <util/delay.h>

#include "ft_spi.h"
#include "ft_uart.h"


int main() {
    uint8_t i = 0;

    init_spi();
    init_uart();

    for (;;) {
        spi_rgb_wheel(i++);
        _delay_ms(5);
    }
}