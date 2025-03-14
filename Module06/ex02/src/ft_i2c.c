#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdlib.h>

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

    // uart_printstr("I2C_START: 0x");
    // uart_print_number_base(TWSR & 0xF8, "0123456789abcdef", 16);
    // uart_printstr("\r\n");

    // Check value of TWI status register. Mask prescaler bits. If status different from START: Error
    // if ((TWSR & 0xF8) != I2C_MASTER_START) uart_printstr("Invalid I2C Master Start.\r\n");
}

inline static void aht20_init(void) {
    i2c_write(AHT20_START_1);
    i2c_write(AHT20_START_2);
    i2c_write(AHT20_START_3);
}

inline void i2c_write(const uint8_t data) {
    // Load DATA into TWDR register. Clear TWINT bit in TWCR to start transmission of data
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait for TWINT flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received.
    while (!(TWCR & (1 << TWINT)));

    // uart_printstr("I2C_TX_DATA Status: 0x");
    // uart_print_number_base(TWSR & 0xF8, "0123456789abcdef", 16);
    // uart_printstr("\r\n");

    // Check value of TWI status register. Mask prescaler bits. If status different from MT_DATA_ACK go to ERROR
    // if ((TWSR & 0xF8) != I2C_DATA_TX) uart_printstr("Invalid I2C Data Status.\r\n");
}

inline void aht20_connect(enum e_i2c_mode mode) {
    // Load Slave into TWDR register (7 Bits) + Write/Read Mode (1 Bit). Clear TWINT bit in TWCR to start transmission of address
    TWDR = (AHT20_ADDRESS << 1) | (mode << 0);
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait for TWINT flag set. This indicates that the SLA+MODE has been transmitted, and ACK/NACK has been received.
    while (!(TWCR & (1 << TWINT)));

    // Check value of TWI status register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
    // uart_printstr("I2C_ADDR_TX_SLV_ACK: 0x");
    // uart_print_number_base(TWSR & 0xF8, "0123456789abcdef", 16);
    // uart_printstr("\r\n");

    // Check value of TWI status register. Mask prescaler bits. If status different from MASTER_TX go to ERROR
    // if ((TWSR & 0xF8) != (mode == I2C_WRITE_MODE ? I2C_ADDR_TX_SLV_ACK : I2C_ADDR_RX_SLV_ACK)) uart_printstr("Invalid I2C Address Transmission ACK.\r\n");
}

inline void i2c_start(void) {
    aht20_connect(I2C_WRITE_MODE);

    aht20_init();
}

inline void i2c_stop(void) {
    // Interruption flag | TWI Enable (enables TWI operation and activates TWI interface) | TWI Stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

inline uint8_t i2c_read(const uint8_t ack) {
    // Load Slave into TWDR register (7 Bits) + Read (1 Bit). Clear TWINT bit in TWCR to start transmission of address
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack << TWEA);

    // Wait for TWINT flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
    while (!(TWCR & (1 << TWINT)));

    return TWDR;
}

inline void print_hex_value(const char c) {
    uint8_t n = (uint8_t) c;

    if (n < 16) uart_tx('0');
    uart_print_number_base(n, "0123456789abcdef", 16);
}

inline static void aht20_trigger_measurement(void) {
    i2c_init();
    aht20_connect(I2C_WRITE_MODE);

    i2c_write(AHT20_MEASURE_1);
    i2c_write(AHT20_MEASURE_2);
    i2c_write(AHT20_MEASURE_3);

    i2c_stop();

    _delay_ms(80);
}

inline static void get_aht_raw_packet(uint8_t *dest) {
    for (;;) {
        i2c_init();
        aht20_connect(I2C_READ_MODE);
        dest[0] = i2c_read(1);

        if (!(TWDR & (1 << 7))) break ;

        i2c_stop();
    }

    for (uint8_t i = 0; i < 6; ++i) {
        dest[i + 1] = i2c_read(i != 5);
    }

    i2c_stop();
}

inline void read_aht_packet_raw(void) {
    uint8_t raw_aht_packet[7] = { 0 };

    aht20_trigger_measurement();

    get_aht_raw_packet((uint8_t *) raw_aht_packet);

    for (uint8_t i = 0; i < 7; ++i) {
        print_hex_value(raw_aht_packet[i]);
        if (i % 6) uart_tx(' ');
    }

    uart_printstr("\r\n");
}

inline double get_temp(const uint8_t raw[7]) {
    uint32_t data = ((((uint32_t) raw[3]) & 0b1111) << 16) | (((uint32_t) raw[4]) << 8) | raw[5];
    const double temp = ((((double) data) / 1048576) * 200) - 50;

    return temp;
}

inline double get_humidity(const uint8_t raw[7]) {
    uint32_t data = (((uint32_t) raw[1]) << 12) | (((uint32_t) raw[2]) << 4) | ((raw[3] & 0b11110000) >> 4);
    const double humidity = (((double) data) / 1048576) * 100;

    return humidity;
}

inline void read_temp_humidity(void) {
    uint8_t raw_aht_packet[7] = { 0 };
    char str[8] = { 0 };
    double temp[3], humidity[3];

    aht20_trigger_measurement();

    for (int i = 0; i < 3; ++i) {
        get_aht_raw_packet((uint8_t *) raw_aht_packet);

        temp[i] = get_temp(raw_aht_packet);
        humidity[i] = get_humidity(raw_aht_packet);
    }


    uart_printstr("Temperature: ");
    uart_printstr(dtostrf((temp[0] + temp[1] + temp[2]) / 3, 4, 1, (char *) &str));
    uart_printstr(" C, Humidity: ");
    uart_printstr(dtostrf((humidity[0] + humidity[1] + humidity[2]) / 3, 4, 1, (char *) &str));

    uart_printstr("%\r\n");
}