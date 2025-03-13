#ifndef __FT_I2C_H__
# define __FT_I2C_H__


# define AHT20_ADDRESS  0x38

# define I2C_BAUDRATE   F_CPU / (16 + (2 * TWBR) + PRESCALAR)
# define I2C_MASTER_START   0x8
# define I2C_ADDR_TX_SLV_ACK      0x18
# define I2C_ADDR_NTX_SLV_ACK     0x20
# define I2C_DATA_TX        0x28
# define I2C_DATA_NTX       0x30


void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void tx_i2c_data(uint8_t data);

#endif // __FT_I2C_H__