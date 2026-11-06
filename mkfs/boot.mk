CC		:= gcc
SRC		:= ../bootloader/boot.c 
CSTD	?= c11
CFLAGS	:= -std=$(CSTD) -Wall -Wextra -ffreestanding -fshort-wchar -fno-stack-protector -mno-red-zone -fpic
OBJ		:= $(SRC:.c=.o)
DEPS	:= $(OBJ:.o=.d)

.PHONY: all
all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)