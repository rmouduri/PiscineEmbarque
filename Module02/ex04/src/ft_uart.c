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

static inline void print_debug(const uint8_t n) {
    const char base[10] = "0123456789";

    uart_printstr(": ");
    uart_print_number(n, base, sizeof(base));
    uart_printstr("\r\n");
}

inline void uart_print_number(int32_t n, const char *base, const uint8_t base_len) {
    if (n == INT32_MIN) {
        return uart_printstr("-2147483648"); // For base 10 only
    } else if (n < 0) {
        n *= -1;
        uart_tx('-');
    }

    if (n >= base_len) {
        uart_print_number(n / base_len, base, base_len);
    }

    uart_tx(base[n % base_len]);
}


volatile uint8_t state = LOGIN_MODE, input_len = 0;
char login[MAX_LOGIN_LENGTH] = { 0 }, pass[MAX_PASS_LENGTH] = { 0 };


static inline void clear_credentials(void) {
    uint8_t i = 0;

    while (i < MAX_LOGIN_LENGTH) login[i++] = 0;

    i = 0;
    while (i < MAX_PASS_LENGTH) pass[i++] = 0;
}

static inline void reset_prompt(void) {
    uart_printstr("Enter your login:\r\n");
    uart_printstr("\tusername: ");
}

void init_prompt(void) {
    clear_credentials();
    reset_prompt();
}

static inline uint8_t check_login(void) {
    uint8_t i = 0;

    while (i < MAX_LOGIN_LENGTH - 2 && login[i + 1] && LOGIN[i + 1]) {
        if (login[i] != LOGIN[i]) {
            return 0;
        }

        ++i;
    }

    ++i;
    return login[i] == LOGIN[i];
}

static inline uint8_t check_pass(void) {
    uint8_t i = 0;

    while (i < MAX_PASS_LENGTH - 2 && pass[i + 1] && PASS[i + 1]) {
        if (pass[i] != PASS[i]) {
            return 0;
        }

        ++i;
    }

    ++i;
    return pass[i] == PASS[i];
}

static inline void handle_enter(void) {
    uart_printstr("\r\n");

    if (state == PASS_MODE) {
        if (check_login() && check_pass()) {
            uart_printstr("Welcome ");
            uart_printstr(login);
            uart_printstr(" !\r\n");
            state = CONNECTED_MODE;
            return ;
        } else {
            uart_printstr("Bad combinaison username/password\r\n\r\n");
            init_prompt();
        }
    } else {
        uart_printstr("\tpassword: ");
    }

    state = !state;
    input_len = 0;
}

static inline void handle_backspace(void) {
    if (input_len) {
        if (state == LOGIN_MODE) login[--input_len] = 0;
        else if (state == PASS_MODE) pass[--input_len] = 0;

        uart_printstr("\b \b");
    }
}

static inline void handle_input(const char c) {
    if (state == CONNECTED_MODE) {
        return ;
    } else if (c == ENTER) {
        handle_enter();
    } else if (c == BACKSPACE) {
        handle_backspace();
    } else if (state == LOGIN_MODE && input_len < MAX_LOGIN_LENGTH) {
        login[input_len++] = c;
        uart_tx(c);
    } else if (state == PASS_MODE && input_len < MAX_PASS_LENGTH) {
        pass[input_len++] = c;
        uart_tx('*');
    }
}

ISR(USART_RX_vect)
{
    char c = uart_rx();

    handle_input(c);
}