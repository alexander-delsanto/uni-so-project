# Compiler and flags
.PHONY: recompile
CC=gcc
CFLAGS=-g -O0 -std=c89 -Wpedantic
CCOMPILE=$(CC) $(CFLAGS)

# Directories
TARGET=master
SRC_DIR=src
LIB_DIR=lib
BIN_DIR=bin

# Binaries and sources
BINARIES = $(TARGET) port ship weather
BINARIES_C=$(addprefix $(SRC_DIR)/, $(addsuffix .c, $(BINARIES)))

# Other modules
CFILES=$(filter-out $(addprefix $(SRC_DIR)/, $(addsuffix .c, $(BINARIES))), $(wildcard $(SRC_DIR)/*.c))
LIBFILES=$(wildcard $(LIB_DIR)/*.c)

all: $(BINARIES)

$(BIN_DIR):
	@mkdir -p $@

$(BINARIES): $(BINARIES_C) | $(BIN_DIR)
	@$(CCOMPILE) $(SRC_DIR)/$@.c $(CFILES) $(LIBFILES) -o $(BIN_DIR)/$@ -lm

# General use
recompile: clean all

clean:
	@$(RM) -r $(BIN_DIR) && ipcrm -a

run: all
	cd bin && ./$(TARGET)

run-and-log: all
	cd bin && ./$(TARGET) | tee ../output.log

# Tools
list:
	@echo Found souce files: $(CFILES)
	@echo
	@echo Found library files: $(LIBFILES)
	@echo
	@echo Found binary c files: $(BINARIES_C)

count:
	@wc -l $(BINARIES_C) $(CFILES) $(SRC_DIR)/include/*.h $(LIB_DIR)/* Makefile

killall-int:
	@killall --signal INT $(BINARIES)

killall-kill:
	@killall --signal KILL $(BINARIES)
