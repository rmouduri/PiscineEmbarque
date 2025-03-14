#include <avr/eeprom.h>
#include <avr/io.h>

#include "ft_eeprom.h"
#include "ft_uart.h"


inline void eeprom_write(const uint8_t data, const uint32_t addr) {
    // Wait for completion of previous write
    while (EECR & (1 << EEPE));

    // Set EEPROM address to write to
    EEAR = addr;
    // Set Data to write to EEPROM
    EEDR = data;

    // Stop interruption while writing to EEPROM
    __asm__ __volatile__ ("cli" ::: "memory");
    // EEPROM Master Write Enable
    EECR |= (1 << EEMPE);
    // Start EEPROM Write
    EECR |= (1 << EEPE);
    // Accept interruption
    __asm__ __volatile__ ("sei" ::: "memory");
}

inline uint8_t eeprom_read(const uint32_t addr) {
    // Wait for completion of previous write
    while (EECR & (1 << EEPE));

    // Set EEPROM address to read
    EEAR = addr;

    // Start EEPROM Read
    EECR |= (1 << EERE);

    return EEDR;
}


inline static uint8_t logN(uint32_t n, const uint8_t logN) {
    uint8_t ret = 1;

    while (n >= logN) {
        n /= logN;
        ++ret;
    }

    return ret;
}

inline static uint8_t is_line_empty(const uint8_t hex_line[16]) {
    for (int i = 0; i < 16; ++i) {
        if (hex_line[i]) return 0;
    }

    return 1;
}

inline void print_eeprom(const uint8_t verbose, const uint16_t min_addr, const uint16_t max_addr, const int16_t red) {
    const char base[16] = "0123456789abcdef";
    uint8_t hex_line[16] = { 0 };
    uint8_t empty = 0;

    if (min_addr > 0x3ff || max_addr > 0x3ff || min_addr > max_addr) {
        uart_printstr(RED"Invalid print_eeprom [min_addr]: 0x");
        uart_print_number_base(min_addr, base, sizeof(base));
        uart_printstr(", [max_addr]: 0x");
        uart_print_number_base(max_addr, base, sizeof(base));
        uart_printstr("\r\n"RESET_COLOR);
        return ;
    }

    for (uint16_t i = min_addr; i <= max_addr; i += 16) {
        const uint8_t line_max = (i <= (max_addr - 16)) ? 16 : ((max_addr - min_addr) % 16) + 1;

        for (uint8_t j = 0; j < line_max; ++j) {
            hex_line[j] = eeprom_read(i + j);
        }

        if (i > min_addr && i <= (max_addr - 16) && is_line_empty(hex_line)) {
            if (!empty) {
                uart_printstr("...\r\n");
                empty = 1;
            }

            continue ;
        } else { empty = 0; }

        // Print EEPROM address
        for (uint8_t j = 0; j < (8 - logN(i, 16)); ++j) {
            uart_tx('0');
        }
        uart_print_number_base(i, base, sizeof(base));

        uart_printstr("  ");

        // Print data as hexa
        for (uint8_t j = 0; j < line_max; ++j) {
            if ((int32_t) (i + j) == red) uart_printstr(RED);

            if (hex_line[j] < 16) { uart_tx('0'); }
            uart_print_number_base(hex_line[j], base, sizeof(base));

            if ((int32_t) (i + j) == red) uart_printstr(RESET_COLOR);

            if ((j + 1) != line_max) {
                if ((j + 1) == 8) { uart_tx(' '); }
                uart_tx(' ');
            }
        }

        if (verbose) {
            // Print text
            for (uint8_t j = line_max; j < 16; ++j) {
                uart_printstr("   ");
                if ((j + 1) == 8) uart_tx(' ');
            }
            uart_printstr("  |");
            for (uint8_t j = 0; j < line_max; ++j) {
                if ((int32_t) (i + j) == red) uart_printstr(RED);
                uart_tx(GET_PRINTABLE_HEX_C(hex_line[j]));
                if ((int32_t) (i + j) == red) uart_printstr(RESET_COLOR);
            }
            uart_tx('|');
        }

        uart_printstr("\r\n");
    }
}