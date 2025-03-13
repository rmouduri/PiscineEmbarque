#include <avr/io.h>
#include <avr/interrupt.h>

#include "ft_uart.h"
#include "ft_rgb.h"


inline void init_uart(void) {
    // http://www.rjhcoding.com/avrc-uart.php : Setting Baud Rate
    // UBRR is a 12-bit register, UBRR0L takes 8 lower bits and UBRR0H takes 4 higher bits
    UBRR0L = UBRRN & 0xFF;
    UBRR0H = UBRRN >> 8;
    
    // Enable UART Transmitter, Receiver and RX Interruption
    UCSR0B |= UART_TRANSMITTER | UART_RECEIVER | UART_RX_INTERRUPT;

    // 8 Data Bits | No Parity Bits | 1 Stop Bit
    UCSR0C |= DATA_BITS_8 | NO_PARITY_BITS | STOP_BIT_1;

    sei();

    uart_tx('#');
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

uint8_t input_len = 0;
char color[COLOR_LENGTH + 1] = { 0 };

static inline void reset_color(void) {
    for (uint8_t i = 0; i < COLOR_LENGTH; ++i) color[i] = 0;
}

static inline void uart_print_number_base(int32_t n, const char *base, const uint8_t base_len) {
    if (n == INT32_MIN) {
        return uart_printstr("-2147483648"); // For base 10 only
    } else if (n < 0) {
        n *= -1;
        uart_tx('-');
    }

    if (n >= base_len) {
        uart_print_number_base(n / base_len, base, base_len);
    }

    uart_tx(base[n % base_len]);
}

static inline uint8_t get_rgb(const char color[2]) {
    uint8_t ret = 0;

    for (int i = 0; i < 2; ++i) {
        if (color[i] >= 'A' && color[i] <= 'F') {
            ret = (16 * ret) + (color[i] - 'A' + 10);
        } else if (color[i] >= 'a' && color[i] <= 'f') {
            ret = (16 * ret) + (color[i] - 'a' + 10);
        } else if (color[i] >= '0' && color[i] <= '9') {
            ret = (16 * ret) + (color[i] - '0');
        }
    }

    return ret;
}

static inline uint8_t check_color(void){
    for (uint8_t i = 0; i < COLOR_LENGTH; ++i) {
        const char c = color[i];

        if (!((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'))) {
            return FAILURE;
        }
    }

    return SUCCESS;
}

static inline void handle_enter(void) {
    if (check_color() == FAILURE) {
        uart_printstr("\r\nInvalid color [");
        uart_printstr(color);
        uart_printstr("].");
    } else {
        set_rgb(get_rgb(&color[0]), get_rgb(&color[2]), get_rgb(&color[4]));
    }

    reset_color();
    input_len = 0;
    uart_printstr("\r\n#");
}

static inline void handle_backspace(void) {
    if (input_len > 0) {
        color[--input_len] = 0;

        uart_printstr("\b \b");
    }
}

static inline void handle_input(const char c) {
    if (c == ENTER) {
        handle_enter();
    } else if (c == BACKSPACE) {
        handle_backspace();
    } else if (input_len < COLOR_LENGTH) {
        color[input_len++] = c;
        uart_tx(c);
    }
}

ISR(USART_RX_vect)
{
    char c = uart_rx();

    handle_input(c);
}