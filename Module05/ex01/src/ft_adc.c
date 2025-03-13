#include <avr/io.h>

#include "ft_adc.h"


inline void init_adc(uint8_t adlar) {
    // https://ece-classes.usc.edu/ee459/library/documents/ADC.pdf

    adlar = adlar ? 1 : 0;

    // Use AVCC
    ADMUX |= (1 << REFS0);

    // 8-Bit result, set to 0 to have 10-Bit result
    ADMUX |= (adlar << ADLAR);

    // Select the input line to connect to (ADC0 for ADC_POT/potentiometer), all bits to 0
    // ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);

    // Enable the ADC Module, mandatory
    ADCSRA |= (1 << ADEN);

    // Prescaler at 128, ADC needs clock between 50KHz and 200KHz; 16MHs / 128 = 125KHz
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}

inline uint16_t read_adc(const uint8_t adc) {
    // Set adc port to read
    ADMUX = (ADMUX & ~0b1111) | adc;

    // Start a conversion with :
    ADCSRA |= (1 << ADSC);
    
    // Wait while conversion is in progress :
    while (ADCSRA & (1 << ADSC));

    // Read result in :
    return (ADMUX & (1 << ADLAR)) ? (ADCH) : (ADCL | (ADCH << 8));
}