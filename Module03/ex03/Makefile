NAME =			ex03
NAME_BIN =		$(NAME).bin
NAME_HEX =		$(NAME).hex


CC =		avr-gcc
OBJ_COPY =	avr-objcopy
FLASH =		avrdude

CFLAGS =	-Wall -Wextra -Werror -Wunused -O3 -mmcu=$(CPU) -DF_CPU=$(F_CPU) -DBAUD_RATE=$(BAUD_RATE)
INCLUDES =	-Iinclude

CPU =		atmega328p
BAUD_RATE = 115200
F_CPU =		16000000UL
PROG_ID =	arduino
PORT =		/dev/ttyUSB0

SRCS_DIR =	src
OBJS_DIR =	.objs

SRCS_FILE =	main.c	\
			ft_uart.c	\
			ft_timer.c	\
			ft_rgb.c

OBJ_FILE =	$(SRCS_FILE:.c=.o)

SRCS	=	$(addprefix $(SRCS_DIR)/,$(SRCS_FILE))
OBJS	=	$(addprefix $(OBJS_DIR)/,$(OBJ_FILE))

RM		=	rm -f
RMDIR	=	rmdir

all:	hex flash

hex: $(NAME_HEX)

flash:
	$(FLASH) -p $(CPU) -c $(PROG_ID) -b $(BAUD_RATE) -P $(PORT) -U flash:w:$(NAME_HEX)

run:
	screen $(PORT) $(BAUD_RATE)

$(NAME_BIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME_BIN)

$(NAME_HEX): $(NAME_BIN)
	$(OBJ_COPY) -O ihex $(NAME_BIN) $(NAME_HEX)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

clean:
	@$(RM) $(OBJS)
	@if [ -d "$(OBJS_DIR)" ]; then rmdir $(OBJS_DIR); fi
	@$(RM) $(NAME_BIN) $(NAME_HEX)

re: clean all

.PHONY: all hex_compile bin_compile hex flash run clean re