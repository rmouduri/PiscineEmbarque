#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "ft_eeprom.h"
#include "ft_uart.h"


#define EEPROM_INPUT_MAX_LEN    84
#define EEPROM_ID   0x1f


typedef struct eeprom_input_s {
    char    input[EEPROM_INPUT_MAX_LEN + 1];
    char    *key;
    char    *val;
    uint8_t input_len;
} eeprom_input_t;



inline static void add_key_value(const char *key, const char *value, uint16_t addr) {
    eeprom_write(EEPROM_ID, addr);
    eeprom_write(EEPROM_ID, addr + 1);

    addr += 2;

    for (int i = 0; key[i]; ++i, ++addr) {
        eeprom_write(key[i], addr);
    }

    eeprom_write(EEPROM_ID, addr);

    ++addr;

    for (int i = 0; value[i]; ++i, ++addr) {
        eeprom_write(value[i], addr);
    }

    eeprom_write(EEPROM_ID, addr);
    eeprom_write(EEPROM_ID, addr + 1);
}

inline static uint8_t eeprom_memcmp(uint16_t addr, const char *cmp) {
    uint8_t i = 0;

    while (i < 32 && addr < 1024 && cmp[i] && eeprom_read(addr) == cmp[i]) {
        ++addr;
        ++i;
    }

    // uart_printstr("[memcmp] i: ");
    // uart_print_number_base(i, "0123456789", 10);
    // uart_printstr(", addr: ");
    // uart_print_number_base(addr, "0123456789abcdef", 16);
    // uart_printstr(", !cmp[i]: ");
    // uart_print_number_base(cmp[i] == 0, "0123456789", 10);
    // uart_printstr(", eeprom_read(addr) == EEPROM_ID: ");
    // uart_print_number_base(eeprom_read(addr) == EEPROM_ID, "0123456789", 10);
    // uart_printstr("\r\n");

    return !(cmp[i] == 0 && eeprom_read(addr) == EEPROM_ID);
}

inline static void eeprom_key_read(const char *key) {
    char val[33] = { 0 };
    uint16_t pos = 0, val_index = 0;

    for (; pos < 1024; ++pos) {
        const uint8_t data = eeprom_read(pos);

        if (data == EEPROM_ID && eeprom_read(pos + 1) == EEPROM_ID) {
            pos += 2;

            if (eeprom_memcmp(pos, key) == 0) {
                break ;
            }
        }
    }

    if (pos >= 1024) {
        uart_printstr("empty\r\n");
        return ;
    }

    pos += strlen(key) + 1;

    while (pos < 1024 && val_index < 32) {
        const char c = eeprom_read(pos);

        if (c == EEPROM_ID && eeprom_read(pos + 1) == EEPROM_ID) break ;

        val[val_index++] = c;

        ++pos;
    }

    if (pos >= 1024 || val_index >= 32) {
        uart_printstr("empty\r\n");
        return ;
    }

    uart_tx('"');
    uart_printstr(val);
    uart_printstr("\"\r\n");
}

inline static void eeprom_key_forget(const char *key) {
    uint16_t pos = 0, val_index = 0;

    for (; pos < 1024; ++pos) {
        const uint8_t data = eeprom_read(pos);

        if (data == EEPROM_ID && eeprom_read(pos + 1) == EEPROM_ID) {
            pos += 2;

            if (eeprom_memcmp(pos, key) == 0) {
                break ;
            }
        }
    }

    if (pos >= 1024) {
        uart_printstr("not found\r\n");
        return ;
    }

    pos += strlen(key) + 1;

    while (pos < 1024 && val_index < 32) {
        const char c = eeprom_read(pos);

        if (c == EEPROM_ID && eeprom_read(pos + 1) == EEPROM_ID) break ;

        ++pos;
        ++val_index;
    }

    if (pos >= 1024 || val_index >= 32) {
        uart_printstr("not found\r\n");
        return ;
    }

    eeprom_write(0x0, pos - val_index - 1 - strlen(key) - 2);
    eeprom_write(0x0, pos - val_index - 1 - strlen(key) - 1);
    eeprom_write(0x0, pos - val_index - 1);
    eeprom_write(0x0, pos);
    eeprom_write(0x0, pos + 1);
}

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
            eeprom_key_read(eeprom_input->key);
        } else if (strncmp(eeprom_input->input, "WRITE", 5) == 0) {
            // eeprom_key_write(eeprom_input->key, eeprom_input->val);
        } else if (strncmp(eeprom_input->input, "FORGET", 6) == 0) {
            eeprom_key_forget(eeprom_input->key);
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

    add_key_value("MY_KEY_42", "MY_VALUE_42", 0x42);

    uart_printstr("> ");
    for (;;) {
        char c = uart_rx();

        handle_input(c, &eeprom_input);
    }
}