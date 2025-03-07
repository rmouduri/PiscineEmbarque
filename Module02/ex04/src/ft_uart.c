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

inline void uart_printstr(const char *s) {
    while (*s > 0) uart_tx(*s++);
}

inline void uart_print_number(int32_t n, const char *base, const uint8_t base_len) {
    if (n == INT32_MIN) {
        return uart_printstr("-2147483647");
    } else if (n < 0) {
        n *= -1;
        uart_tx('-');
    }

    if (n >= base_len) {
        uart_print_number(n / base_len, base, base_len);
    }

    uart_tx((n % base_len) + '0');
}

static inline void handle_input(const char c) {
    if (c == '\r') {
        uart_printstr("\r\n");
    } else if (c == 0x7F) {
        uart_printstr("\r\b");
    } else {
        uart_tx(c);
    }
}

ISR(USART_RX_vect)
{
    char c = uart_rx();

    handle_input(c);

    const char *base = "0123456789";

    uart_printstr(": ");
    uart_print_number(c, base, 10);
    uart_printstr("\r\n");
}