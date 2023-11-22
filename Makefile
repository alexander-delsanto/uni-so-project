# Code directories and main file name
.PHONY: recompile
_MAIN=master
SRC=src
LIB=lib
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
all: master ship port weather# $(BINARIES_OUT)

master: | $(BIN)
	@$(CCOMPILE) $(SRC)/$@.c $(SRC)/shm_general.c $(SRC)/shm_port.c $(SRC)/cargo.c $(SRC)/sem.c $(SRC)/expired.c $(SRC)/utils.c $(SRC)/shm_ship.c $(LIB)/shm.c $(LIB)/semaphore.c -o $(BIN)/$@

ship: | $(BIN)
	@$(CCOMPILE) $(SRC)/$@.c $(SRC)/shm_general.c $(SRC)/shm_port.c $(SRC)/cargo.c $(SRC)/sem.c $(SRC)/expired.c $(SRC)/utils.c $(SRC)/shm_ship.c $(LIB)/shm.c $(LIB)/semaphore.c -o $(BIN)/$@ -lm

port: | $(BIN)
	@$(CCOMPILE) $(SRC)/$@.c $(SRC)/shm_general.c $(SRC)/shm_port.c $(SRC)/cargo.c $(SRC)/sem.c $(SRC)/expired.c $(SRC)/utils.c $(SRC)/shm_ship.c $(LIB)/shm.c $(LIB)/semaphore.c -o $(BIN)/$@ -lm

weather: | $(BIN)
	@$(CCOMPILE) $(SRC)/$@.c $(SRC)/shm_general.c $(SRC)/shm_port.c $(SRC)/cargo.c $(SRC)/sem.c $(SRC)/expired.c $(SRC)/utils.c $(SRC)/shm_ship.c $(LIB)/shm.c $(LIB)/semaphore.c -o $(BIN)/$@ -lm

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
	@$(RM) -r $(BIN) && ipcrm -a