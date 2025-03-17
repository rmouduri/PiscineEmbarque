#ifndef __FT_EXPANDER_H__
# define __FT_EXPANDER_H__

# define EXPANDER_ADDRESS   0b0100000

// Expander commands
# define INPUT_PORT0    0
# define INPUT_PORT1    1
# define OUTPUT_PORT0   2
# define OUTPUT_PORT1   3
# define POLAR_INV_PORT0   4
# define POLAR_INV_PORT1   5
# define CONFIG_PORT0   6
# define CONFIG_PORT1   7

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
# define PORT1_DP   (1 << 7)

#endif // __FT_EXPANDER_H__