#include <avr/io.h>

#include "ft_uart.h"


inline void uart_init(void) {
    // http://www.rjhcoding.com/avrc-uart.php : Setting Baud Rate
    // UBRR is a 12-bit register, UBRR0L takes 8 lower bits and UBRR0H takes 4 higher bits
    UBRR0L = UBRRN & 0xFF;
    UBRR0H = UBRRN >> 8;
    
    // Enable UART Transmitter
    UCSR0B |= UART_TRANSMITTER;

    // 8 Data Bits | No Parity Bits | 1 Stop Bit
    UCSR0C |= DATA_BITS_8 | NO_PARITY_BITS | STOP_BIT_1;
}

inline void uart_tx(const char c) {
    while (!(UCSR0A & (1 << UDRE0)));

    // load data into transmit register
    UDR0 = c;
}