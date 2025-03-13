#include <avr/io.h>
#include <avr/interrupt.h>

#include "ft_uart.h"


inline void uart_init(void) {
    // http://www.rjhcoding.com/avrc-uart.php : Setting Baud Rate
    // UBRR is a 12-bit register, UBRR0L takes 8 lower bits and UBRR0H takes 4 higher bits
    UBRR0L = UBRRN & 0xFF;
    UBRR0H = UBRRN >> 8;
    
    // Enable UART Transmitter, Receiver and RX Interruption
    UCSR0B |= UART_TRANSMITTER | UART_RECEIVER | UART_RX_INTERRUPT;

    // 8 Data Bits | No Parity Bits | 1 Stop Bit
    UCSR0C |= DATA_BITS_8 | NO_PARITY_BITS | STOP_BIT_1;

    sei();
}

inline void uart_tx(const char c) {
    // wait for transmit buffer to be empty
    while (!(UCSR0A & (1 << UDRE0)));

    // load data into transmit register
    UDR0 = c;
}

inline char uart_rx(void) {
    // wait for data
    while(!(UCSR0A & (1 << RXC0)));

    // return data
    return UDR0;
}

ISR(USART_RX_vect)
{
    char c = uart_rx();

    if (c >= 'A' && c <= 'Z') c += 32;
    else if (c >= 'a' && c <= 'z') c -= 32;

    uart_tx(c);
}