#ifndef __FT_I2C_H__
# define __FT_I2C_H__


# include "stdint.h"

# define AHT20_ADDRESS      0x38
# define AHT20_START_1      0xBE
# define AHT20_START_2      0x08
# define AHT20_START_3      0x00
# define AHT20_MEASURE_1    0xAC
# define AHT20_MEASURE_2    0x33
# define AHT20_MEASURE_3    0x00

# define I2C_BAUDRATE           (F_CPU / (16 + (2 * TWBR) + PRESCALAR))
# define I2C_MASTER_START       0x8
# define I2C_ADDR_TX_SLV_ACK    0x18
# define I2C_ADDR_NTX_SLV_ACK   0x20
# define I2C_ADDR_RX_SLV_ACK    0x40
# define I2C_ADDR_NRX_SLV_ACK   0x48
# define I2C_DATA_TX            0x28
# define I2C_DATA_NTX           0x30


enum e_i2c_mode {
    I2C_WRITE_MODE  =   0,
    I2C_READ_MODE   =   1
};


void i2c_init(void);
void aht20_connect(enum e_i2c_mode mode);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(const unsigned char data);
uint8_t i2c_read(const uint8_t ack);
void print_hex_value(const char c);
void read_aht_packet_raw(void);
void read_temp_humidity(void);
void i2c_slave_connect(const uint8_t slave, const enum e_i2c_mode mode);

#endif // __FT_I2C_H__