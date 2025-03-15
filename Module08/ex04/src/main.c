#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>

#include "ft_interrupt.h"
#include "ft_spi.h"
#include "ft_uart.h"


# define LED_FIXED          0x0
# define LED_RAINBOW        0x1
# define INPUT_LENGTH_MAX   12

typedef struct frame_s {
    char input[INPUT_LENGTH_MAX + 1];
    uint8_t input_len;
    led_frame_t d6;
    led_frame_t d7;
    led_frame_t d8;
    uint8_t d6_state;
    uint8_t d7_state;
    uint8_t d8_state;
}   frame_t;

volatile frame_t frame = {
    .input = { 0 },
    .input_len = 0,
    .d6_state = LED_FIXED,
    .d7_state = LED_FIXED,
    .d8_state = LED_FIXED,
    .d6 = DEFAULT_LED_FRAME,
    .d7 = DEFAULT_LED_FRAME,
    .d8 = DEFAULT_LED_FRAME
};


inline void spi_rgb_wheel(uint8_t pos, const frame_t frame) {
    pos = 255 - pos;

    if (pos < 85) {
        set_rgb_spi(
            (frame.d6_state == LED_RAINBOW ? (get_led_frame(255 - pos * 3, 0, pos * 3)) : frame.d6),
            (frame.d7_state == LED_RAINBOW ? (get_led_frame(255 - pos * 3, 0, pos * 3)) : frame.d7),
            (frame.d8_state == LED_RAINBOW ? (get_led_frame(255 - pos * 3, 0, pos * 3)) : frame.d8)
        );
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb_spi(
            (frame.d6_state == LED_RAINBOW ? (get_led_frame(0, pos * 3, 255 - pos * 3)) : frame.d6),
            (frame.d7_state == LED_RAINBOW ? (get_led_frame(0, pos * 3, 255 - pos * 3)) : frame.d7),
            (frame.d8_state == LED_RAINBOW ? (get_led_frame(0, pos * 3, 255 - pos * 3)) : frame.d8)
        );
    } else {
        pos = pos - 170;
        set_rgb_spi(
            (frame.d6_state == LED_RAINBOW ? (get_led_frame(pos * 3, 255 - pos * 3, 0)) : frame.d6),
            (frame.d7_state == LED_RAINBOW ? (get_led_frame(pos * 3, 255 - pos * 3, 0)) : frame.d7),
            (frame.d8_state == LED_RAINBOW ? (get_led_frame(pos * 3, 255 - pos * 3, 0)) : frame.d8)
        );
    }
}

ISR(TIMER1_OVF_vect) {
    extern volatile frame_t frame;
    static uint8_t i = 0;

    spi_rgb_wheel(i++, frame);
}

inline static void prompt(void) {
    uart_tx('#');
}

inline static void handle_backspace(void) {
    if (frame.input_len) {
        frame.input[--frame.input_len] = 0;

        uart_printstr("\b \b");
    }
}

inline static uint8_t is_input_valid(void) {
    if (strncmp((char *)frame.input, "FULLRAINBOW", 12) == 0) return 1;

    for (int i = 0; frame.input[i] && i < 6; ++i) {
        const char c = frame.input[i];

        if (!((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'))) {
            return 0;
        }
    }

    if (frame.input[6] != 'D') return 0;

    if (frame.input[7] != '6' && frame.input[7] != '7' && frame.input[7] != '8') return 0;

    if (frame.input[8]) return 0;

    return 1;
}

inline static uint8_t get_rgb(const char color[2]) {
    uint8_t ret = 0;

    for (int i = 0; i < 2; ++i) {
        if (color[i] >= 'A' && color[i] <= 'F') {
            ret = (16 * ret) + (color[i] - 'A' + 10);
        } else if (color[i] >= 'a' && color[i] <= 'f') {
            ret = (16 * ret) + (color[i] - 'a' + 10);
        } else if (color[i] >= '0' && color[i] <= '9') {
            ret = (16 * ret) + (color[i] - '0');
        }
    }

    return ret;
}

inline static void set_led(void) {
    led_frame_t *led;

    if (frame.input[7] == '6') {
        led = (led_frame_t *) &frame.d6;
        frame.d6_state = LED_FIXED;
    } else if (frame.input[7] == '7') {
        led = (led_frame_t *) &frame.d7;
        frame.d7_state = LED_FIXED;
    } else if (frame.input[7] == '8') {
        led = (led_frame_t *) &frame.d8;
        frame.d8_state = LED_FIXED;
    } else return ;

    led->red = get_rgb((char *) &frame.input[0]);
    led->green = get_rgb((char *) &frame.input[2]);
    led->blue = get_rgb((char *) &frame.input[4]);
}

inline static void handle_enter(void) {
    uart_printstr("\r\n");

    if (is_input_valid()) {
        if (strncmp((char *)frame.input, "FULLRAINBOW", 12) == 0) {
            frame.d6_state = LED_RAINBOW;
            frame.d7_state = LED_RAINBOW;
            frame.d8_state = LED_RAINBOW;
        } else {
            set_led();
        }
    } else {
        uart_printstr(RED"Invalid input.\r\n"RESET_COLOR);
    }

    for (int i = 0; i < frame.input_len; ++i) {
        frame.input[i] = 0;
    }

    frame.input_len = 0;
    prompt();
}

inline static void handle_input(const char c) {
    if (c == ENTER) {
        handle_enter();
    } else if (c == BACKSPACE) {
        handle_backspace();
    } else if (frame.input_len < INPUT_LENGTH_MAX) {
        frame.input[frame.input_len++] = c;
        uart_tx(c);
    }
}

ISR(USART_RX_vect)
{
    const char c = uart_rx();

    handle_input(c);
}

int main() {
    init_spi();
    init_uart();
    init_timer1_interrupt();

    prompt();
    for (;;);
}