#ifndef __FT_UART_H__
# define __FT_UART_H__

# define ROUND(n)   (uint16_t) ((n + .5 >= ((uint16_t) n) + 1) ? (n + 1) : (n))

# define UART_BAUDRATE  BAUD_RATE

// http://www.rjhcoding.com/avrc-uart.php : Setting Baud Rate
// UBBRn = (fosc / (16 * BAUD)) - 1 ; Should be 7.68
# define UBRRN_DOUBLE   (double) (((double)F_CPU / (double)(16 * UART_BAUDRATE)) - 1)
// Round to closest value, here: 8
# define UBRRN          ROUND(UBRRN_DOUBLE)

# define UART_TRANSMITTER   (1 << TXEN0)
# define UART_RECEIVER      (1 << RXEN0)
# define DATA_BITS_8        (3 << UCSZ00)
# define NO_PARITY_BITS     (1 << UPM00)
# define STOP_BIT_1         (0 << USBS0)


void uart_init(void);
void uart_tx(const char c);
char uart_rx(void);
void uart_printstr(const char *s);

#endif // __FT_UART_H__