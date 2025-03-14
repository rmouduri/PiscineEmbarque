#ifndef __FT_RGB_H__
# define __FT_RGB_H__

# include <avr/io.h>

# define LED_D5_RED     PORTD5
# define LED_D5_GREEN   PORTD6
# define LED_D5_BLUE    PORTD3


void set_rgb(const uint8_t r, const uint8_t g, const uint8_t b);
void init_rgb(void);
void wheel(uint8_t pos);

#endif // __FT_RGB_H__