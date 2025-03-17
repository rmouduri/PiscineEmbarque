#ifndef __FT_EXPANDER_H__
# define __FT_EXPANDER_H__

# define EXPANDER_ADDRESS   0b0100000

// Expander commands
# define INPUT_PORT0        0
# define INPUT_PORT1        1
# define OUTPUT_PORT0       2
# define OUTPUT_PORT1       3
# define POLAR_INV_PORT0    4
# define POLAR_INV_PORT1    5
# define CONFIG_PORT0       6
# define CONFIG_PORT1       7

// PORT0
# define PORT0_SW3  (1 << 0)
# define PORT0_D11  (1 << 1)
# define PORT0_D10  (1 << 2)
# define PORT0_D09  (1 << 3)
# define PORT0_CA1  (1 << 4)
# define PORT0_CA2  (1 << 5)
# define PORT0_CA3  (1 << 6)
# define PORT0_CA4  (1 << 7)

// PORT1
# define PORT1_A    (1 << 0)
# define PORT1_B    (1 << 1)
# define PORT1_C    (1 << 2)
# define PORT1_D    (1 << 3)
# define PORT1_E    (1 << 4)
# define PORT1_F    (1 << 5)
# define PORT1_G    (1 << 6)
# define PORT1_DPX  (1 << 7)

// 7-segment
# define SEGM7_N0   (PORT1_A | PORT1_B | PORT1_C | PORT1_D | PORT1_E | PORT1_F)
# define SEGM7_N1   (PORT1_B | PORT1_C)
# define SEGM7_N2   (PORT1_A | PORT1_B | PORT1_G | PORT1_E | PORT1_D)
# define SEGM7_N3   (PORT1_A | PORT1_B | PORT1_G | PORT1_C | PORT1_D)
# define SEGM7_N4   (PORT1_F | PORT1_G | PORT1_B | PORT1_C)
# define SEGM7_N5   (PORT1_A | PORT1_F | PORT1_G | PORT1_C | PORT1_D)
# define SEGM7_N6   (PORT1_A | PORT1_F | PORT1_E | PORT1_D | PORT1_C | PORT1_G)
# define SEGM7_N7   (PORT1_A | PORT1_B | PORT1_C)
# define SEGM7_N8   (PORT1_A | PORT1_B | PORT1_C | PORT1_D | PORT1_E | PORT1_F | PORT1_G )
# define SEGM7_N9   (PORT1_A | PORT1_F | PORT1_G | PORT1_B | PORT1_C | PORT1_D)

# define SEGM7_TAB  { SEGM7_N0, SEGM7_N1, SEGM7_N2, SEGM7_N3, SEGM7_N4, SEGM7_N5, SEGM7_N6, SEGM7_N7, SEGM7_N8, SEGM7_N9 }
# define SEGM7_CAN  { PORT0_CA4, PORT0_CA3, PORT0_CA2, PORT0_CA1 }


uint8_t get_sw3(void);
void turn_on_7segment(const uint16_t n, const uint8_t display_left_numbers);

#endif // __FT_EXPANDER_H__