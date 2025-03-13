#include <avr/io.h>
#include <util/twi.h>

#include "ft_i2c.h"
#include "ft_uart.h"


inline void i2c_init(void) {
    // (16MHz) / (16 + ((2 * TWBR) * PRESCALE) = 100kHz
    // (16MHz) / (16 + ((2 * 18) * 4) = 100kHz
    TWBR = 18;
    // Set prescalar to 4
    TWSR |= (1 << TWPS0);

    // Interruption flag | TWI Enable (enables TWI operation and activates TWI interface) | TWI Start condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);

    // Wait for TWINT flag set. This indicates that the START condition has been transmitted
    while (!(TWCR & (1 << TWINT)));

    uart_printstr("I2C_START: 0x");
    uart_print_number_base(TWSR & 0xF8, "0123456789abcdef", 16);
    uart_printstr("\r\n");

    // Check value of TWI status register. Mask prescaler bits. If status different from START: Error
    if ((TWSR & 0xF8) != I2C_MASTER_START) uart_printstr("Invalid I2C Master Start.\r\n");
}

inline void tx_i2c_data(uint8_t data) {
    // Load DATA into TWDR register. Clear TWINT bit in TWCR to start transmission of data
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait for TWINT flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received.
    while (!(TWCR & (1 << TWINT)));

    uart_printstr("I2C_TX_DATA Status: 0x");
    uart_print_number_base(TWSR & 0xF8, "0123456789abcdef", 16);
    uart_printstr("\r\n");

    // Check value of TWI status register. Mask prescaler bits. If status different from MT_DATA_ACK go to ERROR
    if ((TWSR & 0xF8)!= I2C_DATA_TX) uart_printstr("Invalid I2C Data Status.\r\n"); ;
}

inline void i2c_start(void) {
    // Load Slave into TWDR register (7 Bits) + Write (1 Bit). Clear TWINT bit in TWCR to start transmission of address
    TWDR = (AHT20_ADDRESS << 1) | (0 << 0);
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait for TWINT flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
    while (!(TWCR & (1 << TWINT)));

    // Check value of TWI status register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
    uart_printstr("I2C_ADDR_TX_SLV_ACK: 0x");
    uart_print_number_base(TWSR & 0xF8, "0123456789abcdef", 16);
    uart_printstr("\r\n");

    // Check value of TWI status register. Mask prescaler bits. If status different from MASTER_TX go to ERROR
    if ((TWSR & 0xF8) != I2C_ADDR_TX_SLV_ACK) uart_printstr("Invalid I2C Slave Address Transmission ACK.\r\n");

    tx_i2c_data(0);
}

inline void i2c_stop(void) {
    // Interruption flag | TWI Enable (enables TWI operation and activates TWI interface) | TWI Stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}