CC		:= gcc
SRC		:= /../bootloader/boot.c 
CFLAGS	:= -std=c11 -Wall -Wextra -ffreestanding -fshort-wchar -fno-stack-protector -mno-red-zone -fpic
OBJ		:= ($SRC:.c=.o)
DEPS	:= ($OBJ:.o=.d)

.PHONY: all
all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)