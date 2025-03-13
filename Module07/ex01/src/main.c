#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "ft_eeprom.h"
#include "ft_uart.h"


#define EEPROM_INPUT_MAX_LEN    8


typedef struct eeprom_input_s {
    char    input[EEPROM_INPUT_MAX_LEN + 1];
    uint8_t input_len;
} eeprom_input_t;


inline static void handle_eeprom_write(char *c_addr) {
    uint8_t i = 0;
    uint32_t data = 0;
    uint32_t addr = 0;

    while (c_addr[i] && c_addr[i] != ' ') {
        const char c = c_addr[i];

        if (c >= 'A' && c <= 'F') {
            addr = (16 * addr) + (c - 'A' + 10);
        } else if (c >= 'a' && c <= 'f') {
            addr = (16 * addr) + (c - 'a' + 10);
        } else if (c >= '0' && c <= '9') {
            addr = (16 * addr) + (c - '0');
        } else {
            uart_printstr(RED"Invalid eeprom write [addr] at: `");
            uart_printstr(&c_addr[i]);
            uart_printstr("`\r\n"RESET_COLOR);
            return ;
        }

        ++i;
    }

    if (addr > 0x3ff) {
        uart_printstr(RED"Invalid eeprom write [addr] : 0x");
        uart_print_number_base(addr, "0123456789abcdef", 16);
        uart_printstr("\r\n"RESET_COLOR);
        return ;
    }

    if (!c_addr[i]) {
        uart_printstr(RED"Invalid eeprom write: empty data.\r\n"RESET_COLOR);
        return ;
    }

    ++i;

    while (c_addr[i] && c_addr[i] != ' ') {
        const char c = c_addr[i];

        if (c >= 'A' && c <= 'F') {
            data = (16 * data) + (c - 'A' + 10);
        } else if (c >= 'a' && c <= 'f') {
            data = (16 * data) + (c - 'a' + 10);
        } else if (c >= '0' && c <= '9') {
            data = (16 * data) + (c - '0');
        } else {
            uart_printstr(RED"Invalid eeprom write [data] at: `");
            uart_printstr(&c_addr[i]);
            uart_printstr("`\r\n"RESET_COLOR);
            return ;
        }

        ++i;
    }

    if (data > 0xff) {
        uart_printstr(RED"Invalid eeprom write [data] : 0x");
        uart_print_number_base(data, "0123456789abcdef", 16);
        uart_printstr("\r\n"RESET_COLOR);
        return ;
    }

    if (eeprom_read(addr) != data) {
        uint16_t low = ((addr >= 32) ? (addr - 32) : 0);
        uint16_t high = ((addr + 32 <= 0x3ff) ? (addr + 32) : 0x3ff);

        low = low >= (low % 16) ? (low - (low % 16)) : low;
        high = (high + (16 - (high % 16)) - 1) <= 0x3ff ? (high + (16 - (high % 16)) - 1) : 0x3ff;
        eeprom_write(data, addr);
        print_eeprom(0, low, high, addr);
    }
}

inline static void handle_enter(eeprom_input_t *eeprom_input) {
    uart_printstr("\r\n");

    if ((eeprom_input->input[0] >= 'A' && eeprom_input->input[0] <= 'F')
            || (eeprom_input->input[0] >= 'a' && eeprom_input->input[0] <= 'f')
            || (eeprom_input->input[0] >= '0' && eeprom_input->input[0] <= '9')) {
        handle_eeprom_write(eeprom_input->input);
    } else {
        uart_printstr(RED"Invalid eeprom write: invalid buffer.\r\n"RESET_COLOR);
    }

    for (int i = 0; i < eeprom_input->input_len; ++i) {
        eeprom_input->input[i] = 0;
    }

    eeprom_input->input_len = 0;
    uart_printstr("> ");
}

inline static void handle_backspace(eeprom_input_t *eeprom_input) {
    if (eeprom_input->input_len) {
        eeprom_input->input[--eeprom_input->input_len] = 0;

        uart_printstr("\b \b");
    }
}

inline static void handle_input(const char c, eeprom_input_t *eeprom_input) {
    if (c == ENTER) {
        handle_enter(eeprom_input);
    } else if (c == BACKSPACE) {
        handle_backspace(eeprom_input);
    } else if (eeprom_input->input_len < EEPROM_INPUT_MAX_LEN) {
        eeprom_input->input[eeprom_input->input_len++] = c;
        uart_tx(c);
    }
}

int main() {
    eeprom_input_t   eeprom_input = { 0 };

    init_uart();

    uart_printstr("> ");
    for (;;) {
        char c = uart_rx();

        handle_input(c, &eeprom_input);
    }
}