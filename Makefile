# Code directories and main file name
.PHONY: recompile
_MAIN=master
SRC=src
BIN=bin
BINARIES=$(_MAIN) port ship weather

# Compiling setup
CC=gcc
CFLAGS=-g -O0 -std=c89 -Wpedantic
CCOMPILE=$(CC) $(CFLAGS)

# Files lists
BINARIES_SRC=$(addprefix $(SRC)/, $(addsuffix .c, $(BINARIES)))
BINARIES_OUT=$(addprefix $(BIN)/, $(BINARIES))
CFILES=$(wildcard $(SRC)/*.c)
REQUIRED=$(filter-out $(BINARIES_SRC), $(CFILES))
REQUIRED_O=$(patsubst src/%.c, $(BIN)/%.o, $(REQUIRED))


# Main
all: $(BINARIES_OUT)

$(BIN):
	@mkdir -p $@

$(BIN)/%.o: $(SRC)/%.c
	$(CCOMPILE) -c $^ -o $@

$(BIN)/%: $(SRC)/%.c $(REQUIRED_O) | $(BIN)
	$(CCOMPILE) $(REQUIRED_O) $< -o $@


#$(BIN)/%.o: src/lib/%.c | $(BIN)
#	$(CCOMPILE) -c $^ -o $@

list:
	@echo Found souce files: $(CFILES)
	@echo Object files: $(REQUIRED_O)
	@echo $(BINARIES_SRC)
	@echo $(BINARIES_OUT)
	@echo $(REQUIRED)

# General use
recompile: clean all
clean:
	@$(RM) -r $(BIN)