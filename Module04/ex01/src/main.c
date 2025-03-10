#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ft_interrupt.h"
#include "ft_uart.h"

int main() {
    // LED D2 Receive
    DDRB |= (1 << DDB1);

    init_uart();
    init_interrupt();

    for (;;);
}