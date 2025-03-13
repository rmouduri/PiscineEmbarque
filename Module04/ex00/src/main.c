#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ft_interrupt.h"

int main() {
    // LED D1 Receive
    DDRB |= (1 << DDB0);

    init_interrupt();

    for (;;);
}