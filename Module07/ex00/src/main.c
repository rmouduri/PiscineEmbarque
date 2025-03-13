#include <avr/io.h>
#include <util/delay.h>

#include "ft_eeprom.h"
#include "ft_uart.h"


int main() {
    init_uart();

    print_eeprom(0);

    for (;;);
}