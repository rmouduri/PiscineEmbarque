#ifndef __FT_ADC_H__
# define __FT_ADC_H__

# include <avr/io.h>

# define ADC_POT    (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0)   // ADC0
# define ADC_LDR    (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0)   // ADC1
# define ADC_NTC    (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0)   // ADC2

void init_adc(uint8_t adlar);
uint16_t read_adc(const uint8_t adc);

#endif // __FT_ADC_H__