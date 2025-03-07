#ifndef __FT_RGB_H__
# define __FT_RGB_H__

# include <avr/io.h>

# define LED_D5_RED     PORTD5
# define LED_D5_GREEN   PORTD6
# define LED_D5_BLUE    PORTD3


# define COLOR_ARRAY    { \
 /* Red */       {0xff, 0, 0}, \
 /* Green */     {0, 0xff, 0}, \
 /* Blue */      {0, 0, 0xff}, \
 /* Yellow */    {0xff, 0xff, 0}, \
 /* Cyan */      {0, 0xff, 0xff}, \
 /* Magenta */   {0xff, 0, 0xff}, \
 /* White */     {0xff, 0xff, 0xff}, \
}


void set_rgb(const uint8_t r, const uint8_t g, const uint8_t b);

#endif // __FT_RGB_H__