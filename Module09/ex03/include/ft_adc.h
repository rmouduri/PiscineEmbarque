#ifndef __FT_ADC_H__
# define __FT_ADC_H__

# include <avr/io.h>

# define ADC_POT            (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0)   // ADC0
# define ADC_LDR            (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0)   // ADC1
# define ADC_NTC            (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0)   // ADC2
# define ADC_TEMP_SENSOR    (1 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0)   // ADC8

// https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
// 23.8.1 Manufacturing Calibration
# define COMPUTED_TS_OFFSET         21.0    // Can't get the value from the board, must compute then with the Table 23.2 and the formula
# define COMPUTED_TS_GAIN           163.84  // Can't get the value from the board, must compute then with the Table 23.2 and the formula
# define ARBITRARY_TEMP_CALIBRATION 45      // Computed interpreting the current value like 25°C
# define GET_TEMP(READ_TEMP)    (int16_t) ((double)((READ_TEMP - ARBITRARY_TEMP_CALIBRATION - (273 + 100 - COMPUTED_TS_OFFSET)) * 128) / COMPUTED_TS_GAIN) + 25


void init_adc(uint8_t adlar, uint8_t internal_mem);
uint16_t read_adc(const uint8_t adc);

#endif // __FT_ADC_H__