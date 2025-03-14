#ifndef __FT_SPI_H__
# define __FT_SPI_H__


typedef struct led_frame_s {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t intensity: 5;
    uint8_t led_frame_start: 3;
}   led_frame_t;

# define APA102_START       0x00000000
# define APA102_END         0xFFFFFFFF
# define LED_FRAME_START    0b111
# define LED_INTENSITY      0x4

void init_spi(void);
void send_spi(const uint32_t data);
led_frame_t get_led_frame(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t intensity);
void set_rgb_spi(const led_frame_t d6, const led_frame_t d7, const led_frame_t d8);
void spi_rgb_wheel(uint8_t pos);

#endif // __FT_SPI_H__