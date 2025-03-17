#include <avr/io.h>
#include <stdint.h>

#include "ft_spi.h"
#include "ft_uart.h"


inline void init_spi(void) {
    // SS | MOSI | SCK
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);

    // Enable SPI, Master, set clock rate fck/16 (Needs to be between 800kHz and 1200kHz: 16MHz / 16 = 1000kHz: OK !)
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

inline void send_spi(const uint32_t data) {
    for (int i = 0; i < 4; ++i) {        
        /* Start transmission */
        SPDR = data >> (8 * (3 - i));

        /* Wait for transmission complete */
        while (!(SPSR & (1 << SPIF)));
    }
}

inline led_frame_t get_led_frame(const uint8_t r, const uint8_t g, const uint8_t b/*, const uint8_t intensity*/) {
    const uint8_t intensity = LED_INTENSITY;

    led_frame_t lf = {
        .led_frame_start = LED_FRAME_START
    };

    if (intensity > 0b11111) {
        uart_printstr(RED"Intensity is too high.\r\n"RESET_COLOR);
        return lf;
    }

    lf.red = r;
    lf.green = g;
    lf.blue = b;
    lf.intensity = intensity;

    return lf;
}

inline void set_rgb_spi(const led_frame_t d6, const led_frame_t d7, const led_frame_t d8) {
    send_spi(APA102_START);

    send_spi(*((uint32_t *) &d6));
    send_spi(*((uint32_t *) &d7));
    send_spi(*((uint32_t *) &d8));

    send_spi(APA102_END);
}