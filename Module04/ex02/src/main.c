#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ft_interrupt.h"


int main() {
    // LED D1, D2, D3, D4 Receive
    DDRB |= (1 << DDB0) | (1 << DDB1)| (1 << DDB2) | (1 << DDB4);

    init_interrupt();

    for (;;);
}