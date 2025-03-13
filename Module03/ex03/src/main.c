#include <avr/io.h>
#include <util/delay.h>

#include "ft_rgb.h"
#include "ft_uart.h"


int main() {
    init_rgb();
    init_uart();

    for (;;);
}