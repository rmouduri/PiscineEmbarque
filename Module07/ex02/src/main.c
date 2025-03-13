#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "ft_eeprom.h"
#include "ft_uart.h"


#define EEPROM_INPUT_MAX_LEN    84


typedef struct eeprom_input_s {
    char    input[EEPROM_INPUT_MAX_LEN + 1];
    char    *key;
    char    *val;
    uint8_t input_len;
} eeprom_input_t;


inline static uint8_t get_val(eeprom_input_t *ei) {
    uint8_t i = 0, length = 0;

    while (i < EEPROM_INPUT_MAX_LEN && ei->input[i]) ++i;

    if (i == EEPROM_INPUT_MAX_LEN) {
        uart_printstr(RED"Value empty.\r\n"RESET_COLOR);
        return 0;
    }

    ++i;
    while (i < EEPROM_INPUT_MAX_LEN && ei->input[i] && ei->input[i] != '"') ++i;

    if (i == EEPROM_INPUT_MAX_LEN || !ei->input[i]) {
        uart_printstr(RED"Value empty.\r\n"RESET_COLOR);
        return 0;
    } else if (ei->input[i] != '"') {
        uart_printstr(RED"Value must be between quotes.\r\n"RESET_COLOR);
        return 0;
    }

    ++i;

    while (ei->input[i] && ei->input[i] != '"' && i < EEPROM_INPUT_MAX_LEN) {
        ++i;
        ++length;
    }

    if (!length || length > 32) {
        uart_printstr(RED"Value length must be >= 1 and <= 32.\r\n"RESET_COLOR);
        return 0;
    } else if (!ei->input[i]) {
        uart_printstr(RED"Value doesn't end with \".\r\n"RESET_COLOR);
        return 0;
    }

    ei->input[i] = 0;
    ei->val = &ei->input[i - length];

    return 1;
}

inline static int get_key(eeprom_input_t *ei) {
    uint8_t i = 0;
    uint8_t length = 0;

    while (i < EEPROM_INPUT_MAX_LEN && ei->input[i] && ei->input[i] != '"') {
        ++i;
    }

    if (ei->input[i] != '"') {
        uart_printstr(RED"Key must be between quotes.\r\n"RESET_COLOR);
        return 0;
    }
    
    ++i;
    while (ei->input[i] && ei->input[i] != '"' && i < EEPROM_INPUT_MAX_LEN) {
        ++i;
        ++length;
    }

    if (!length || length > 32) {
        uart_printstr(RED"Key length must be >= 1 and <= 32.\r\n"RESET_COLOR);
        return 0;
    } else if (!ei->input[i]) {
        uart_printstr(RED"Key doesn't end with \".\r\n"RESET_COLOR);
        return 0;
    }

    ei->input[i] = 0;
    ei->key = &ei->input[i - length];

    return 1;
}

inline static int get_key_val(eeprom_input_t *eeprom_input) {
    if ((strncmp(eeprom_input->input, "WRITE", 5) == 0)
            || (strncmp(eeprom_input->input, "READ", 4) == 0)
            || (strncmp(eeprom_input->input, "FORGET", 6) == 0)) {
        if (!get_key(eeprom_input)) {
            return 0;
        }
    }

    if (strncmp(eeprom_input->input, "WRITE", 5) == 0 && !get_val(eeprom_input)) {
        return 0;
    }

    return 1;
}

inline static void handle_enter(eeprom_input_t *eeprom_input) {
    uart_printstr("\r\n");

    if (get_key_val(eeprom_input)) {
        if (strncmp(eeprom_input->input, "READ", 4) == 0) {
            // eeprom_key_read(eeprom_input->key);
        } else if (strncmp(eeprom_input->input, "WRITE", 5) == 0) {
            // eeprom_key_write(eeprom_input->key, eeprom_input->val);
        } else if (strncmp(eeprom_input->input, "FORGET", 6) == 0) {
            // eeprom_key_read(eeprom_input->key);
        } else if (strncmp(eeprom_input->input, "PRINT", 5) == 0) {
            print_eeprom(1, 0x0, 0x3ff, -1);
        } else {
            uart_printstr(RED"Invalid command.\r\n"RESET_COLOR);
        }
    }

    for (int i = 0; i < eeprom_input->input_len; ++i) {
        eeprom_input->input[i] = 0;
    }

    eeprom_input->input_len = 0;
    eeprom_input->key = 0;
    eeprom_input->val = 0;
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