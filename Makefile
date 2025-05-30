##
## EPITECH PROJECT, 2024
## __
## File description:
## ./Makefile
##

ifeq ($(USE_42),1)
SHELL := /home/savalet/epitech/42sh/d
endif

MAKEFLAGS += -j

BIN_NAME := 42sh

LIB_NAME := libu.a

SRC := $(wildcard src/*.c)
SRC += $(wildcard src/builtins/*.c)
SRC += $(wildcard src/builtins/expr/*.c)
SRC += $(wildcard src/builtins/history/*.c)
SRC += $(wildcard src/ast/*.c)
SRC += $(wildcard src/utils/*.c)
SRC += $(wildcard src/local/*.c)
SRC += $(wildcard src/repl/*.c)

LIB_SRC := $(wildcard ulib/*.c)
LIB_SRC += $(wildcard ulib/write/printf/*.c)
LIB_SRC += $(wildcard ulib/math/*.c)
LIB_SRC += $(wildcard ulib/mem/*.c)
LIB_SRC += $(wildcard ulib/str/*.c)
LIB_SRC += $(wildcard ulib/write/*.c)

BUILD_DIR := .build

CC := gcc

CFLAGS += -Wall -Wextra -Werror=write-strings -iquote ulib
CFLAGS += -Wno-unused-parameter -Wunused-result
CFLAGS += -Wp,-U_FORTIFY_SOURCE -Wcast-qual -Wduplicated-branches
CFLAGS += -Wduplicated-cond -Wformat=2 -Wshadow -fno-builtin
CFLAGS += -Wstrict-aliasing=0 -Wstrict-prototypes -Wunreachable-code
CFLAGS += -Wwrite-strings -Werror=declaration-after-statement
CFLAGS += -Werror=format-nonliteral -Werror=int-conversion -Werror=return-type
CFLAGS += -Wno-discarded-qualifiers --std=gnu2x

LDFLAGS += -L .
LDLIBS := -lu

include utils.mk

.PHONY: _start all
_start: all

# call mk-profile release, SRC, additional CFLAGS
define mk-profile

NAME_$(strip $1) := $4
OBJ_$(strip $1) := $$($(strip $2):%.c=$$(BUILD_DIR)/$(strip $1)/%.o)

LIB_NAME_$(strip $1) := $(BUILD_DIR)/$(strip $1)/$(LIB_NAME)
LIB_OBJ_$(strip $1) := $$(LIB_SRC:%.c=$$(BUILD_DIR)/$(strip $1)/%.o)

$$(BUILD_DIR)/$(strip $1)/%.o: %.c
	@ mkdir -p $$(dir $$@)
	@ $$(COMPILE.c) $$< -o $$@
	@ $$(LOG_TIME) "$$(C_GREEN) CC $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"

$$(LIB_NAME_$(strip $1)): $$(LIB_OBJ_$(strip $1))
	@ ar rc $$@ $$(LIB_OBJ_$(strip $1))
	@ $$(LOG_TIME) "$$(C_CYAN) AR $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"

$$(NAME_$(strip $1)): CFLAGS += -L $$(BUILD_DIR)/$(strip $1) $3
$$(NAME_$(strip $1)): $$(LIB_NAME_$(strip $1)) $$(OBJ_$(strip $1))
	@ $$(LINK.c) $$(OBJ_$(strip $1)) $$(LDFLAGS) $$(LDLIBS) -o $$@
	@ $$(LOG_TIME) "$$(C_GREEN) CC $$(C_PURPLE) $$(notdir $$@) $$(C_RESET)"
	@ $$(LOG_TIME) "$$(C_GREEN) OK  Compilation finished $$(C_RESET)"

endef

$(eval $(call mk-profile, release, SRC, -iquote src -g3, $(BIN_NAME)))
$(eval $(call mk-profile, debug, SRC, -iquote src -D U_DEBUG_MODE \
	-fanalyzer -g3, debug))
$(eval $(call mk-profile, test, SRC, -iquote src --coverage, test))
$(eval $(call mk-profile, afl, SRC, -iquote src -D AFL_MODE, afl_runner))

all: $(NAME_release)

.PHONY: tests_run
tests_run: $(NAME_test)
	@ - python3 validator.py ./$(NAME_test)

.PHONY: cov
cov: tests_run
	gcovr . \
		--gcov-ignore-errors=no_working_dir_found \
		--exclude-unreachable-branches \
		--exclude tests

.PHONY: afl
afl: CC := AFL_USE_ASAN=1 afl-gcc-fast
afl: $(NAME_afl)

define newline


endef

AFL_FLAGS := -i afl/inputs
AFL_FLAGS += -x afl/tokens
AFL_FLAGS += -o afl/generated

PROCS ?= $(shell nproc)

.PHONY: afl_run
afl_run: afl
	@ mkdir -p afl/generated
	screen -dmS main_instance \
		afl-fuzz $(AFL_FLAGS) -M fuzzer_1 -- ./afl_runner
	$(foreach instance, $(shell seq 1 $(PROCS)),\
		screen -dmS afl_$(instance) \
		afl-fuzz $(AFL_FLAGS) -S fuzzer_$(instance) -- ./afl_runner$(newline))
	watch -n 0.25 -- afl-whatsup -s afl/generated

clean:
	@ $(RM) $(OBJ)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(OBJ) $(C_RESET)"

fclean:
	@ $(RM) -r $(NAME_release) $(NAME_debug) $(BUILD_DIR)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(NAME_release) $(NAME_debug) \
		$(C_RESET)"

.NOTPARALLEL: re
re:	fclean all

.PHONY: all clean fclean re

PREFIX ?=
BINDIR ?= $(PREFIX)/bin

.PHONY: install uninstall
install: $(NAME_release)
	install -Dm755 -t $(BINDIR) $(NAME_release)

uninstall:
	$(RM) $(BINDIR)/42sh
