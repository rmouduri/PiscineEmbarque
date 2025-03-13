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

    // EEPROM Master Write Enable
    EECR |= (1 << EEMPE);
    // Start EEPROM Write
    EECR |= (1 << EEPE);
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

inline void print_eeprom(const uint8_t verbose) {
    const char base[16] = "0123456789abcdef";
    uint8_t hex_line[16] = { 0 };

    for (uint16_t i = 0; i < 64; ++i) {
        for (uint8_t j = 0; j < 16; ++j) {
            hex_line[j] = eeprom_read((i * 16) + j);
        }

        // Print EEPROM address
        for (uint8_t j = 0; j < (8 - logN(i * 16, 16)); ++j) {
            uart_tx('0');
        }
        uart_print_number_base(i * 16, base, sizeof(base));

        uart_printstr("  ");

        // Print data as hexa
        for (uint8_t j = 0; j < 16; ++j) {
            if (hex_line[j] < 16) { uart_tx('0'); }
            uart_print_number_base(hex_line[j], base, sizeof(base));

            if (j + 1 != 16) {
                if (j && ((j + 1) % 8) == 0) { uart_tx(' '); }
                uart_tx(' ');
            }
        }

        if (verbose) {
            // Print text
            uart_printstr("  |");
            for (uint8_t j = 0; j < 16; ++j) {
                uart_tx(GET_PRINTABLE_HEX_C(hex_line[j]));
            }
            uart_tx('|');
        }

        uart_printstr("\r\n");
    }
}