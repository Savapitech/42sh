##
## EPITECH PROJECT, 2024
## radar
## File description:
## ./Makefile
##

MAKEFLAGS += -j

NAME := asm

LIB_NAME := libu.a

SRC := $(wildcard src/*.c)

LIB_SRC := $(wildcard ulib/*.c)
LIB_SRC += $(wildcard ulib/write/printf/*.c)
LIB_SRC += $(wildcard ulib/math/*.c)
LIB_SRC += $(wildcard ulib/mem/*.c)
LIB_SRC += $(wildcard ulib/str/*.c)
LIB_SRC += $(wildcard ulib/write/*.c)

BUILD_DIR := .build

OBJ := $(SRC:%.c=$(BUILD_DIR)/%.o)
LIB_OBJ := $(LIB_SRC:%.c=$(BUILD_DIR)/%.o)

CC := gcc

CFLAGS += -Wall -Wextra -Werror=write-strings -iquote ulib
CFLAGS += -Wno-unused-parameter -Wunused-result -fanalyzer
CFLAGS += -Wp,-U_FORTIFY_SOURCE -Wcast-qual -Wduplicated-branches
CFLAGS += -Wduplicated-cond -Wformat=2 -Wshadow
CFLAGS += -Wstrict-aliasing=0 -Wstrict-prototypes -Wunreachable-code
CFLAGS += -Wwrite-strings -Werror=declaration-after-statement
CFLAGS += -Werror=format-nonliteral -Werror=int-conversion -Werror=return-type
CFLAGS += -Werror=vla-larger-than=0 -Wno-discarded-qualifiers

DEBUG_FLAGS := -DR_DEBUG_MODE -g3

LDFLAGS += -L .
LDLIBS := -lu

include utils.mk

all: $(NAME)

$(BUILD_DIR)/%.o: %.c
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) -o $@ -c $<
	@ $(LOG_TIME) "$(C_GREEN) CC $(C_PURPLE) $(notdir $@) $(C_RESET)"

$(LIB_NAME): $(LIB_OBJ)
	@ ar rc $(LIB_NAME) $(LIB_OBJ)
	@ $(LOG_TIME) "$(C_CYAN) AR $(C_PURPLE) $(notdir $@) $(C_RESET)"

$(NAME): $(LIB_NAME) $(OBJ)
	@ $(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	@ $(LOG_TIME) "$(C_GREEN) CC $(C_PURPLE) $(notdir $@) $(C_RESET)"
	@ $(LOG_TIME) "$(C_GREEN) OK  Compilation finished $(C_RESET)"

clean:
	@ $(RM) $(OBJ)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(OBJ) $(C_RESET)"

fclean:
	@ $(RM) -r $(NAME) $(LIB_NAME) $(BUILD_DIR)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(NAME) $(BUILD_DIR) $(C_RESET)"

.NOTPARALLEL: re
re:	fclean all

.NOTPARALLEL: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

.PHONY: all clean fclean re
