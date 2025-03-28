NAME =			ex01
NAME_BIN =		$(NAME).bin
NAME_HEX =		$(NAME).hex


CC =		avr-gcc
OBJ_COPY =	avr-objcopy
FLASH =		avrdude

CFLAGS =	-Wall -Werror -Wextra -Wunused -mmcu=$(CPU) -DF_CPU=$(F_CPU)
INCLUDES =	-Iinclude

CPU =		atmega328p
BAUD_RATE = 115200
F_CPU =		16000000UL
PROG_ID =	arduino

SRCS_DIR =	src
OBJS_DIR =	.objs

SRCS_FILE =	main.c

OBJ_FILE =	$(SRCS_FILE:.c=.o)

SRCS	=	$(addprefix $(SRCS_DIR)/,$(SRCS_FILE))
OBJS	=	$(addprefix $(OBJS_DIR)/,$(OBJ_FILE))

RM		=	rm -f
RMDIR	=	rmdir

all:	hex flash

hex: $(NAME_HEX)

flash:
	$(FLASH) -p $(CPU) -c $(PROG_ID) -b $(BAUD_RATE) -P /dev/ttyUSB0 -U flash:w:$(NAME_HEX)

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

.PHONY: all hex_compile bin_compile hex flash clean re