# ----------------------------------------
# Project definitions
# ----------------------------------------

EXEC = 15puzzle

# ----------------------------------------
# Compiler and linker definitions
# ----------------------------------------

# Compiler and linker
ifndef CC
	CC := gcc
endif

# Flags for compiler
CFLAGS = -W -Wall -Wextra -pedantic -Wconversion -Wswitch-enum -Werror -flto -std=c99

# ----------------------------------------
# Compilation and linking rules
# ----------------------------------------

all: $(EXEC)

$(EXEC): main.c Makefile
	$(CC) $(CFLAGS) $(filter %.c %.s %.o,$^) -o $@

# ----------------------------------------
# Script rules
# ----------------------------------------

clean:
	git clean -fdx

remade: clean all

.PHONY: all clean remade

# ----------------------------------------
