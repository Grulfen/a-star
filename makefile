CC = gcc
CFLAGS = -Wall -Wextra -W -pedantic -std=c99

ifeq ($(mode),release)
	CFLAGS += -O2
else
	CFLAGS += -g
endif

BIN = bin
SRC = src
LDFLAGS = -lncurses

EXE = main

OBJ = $(BIN)/main.o $(BIN)/thing_util.o

all: mkbin $(EXE) 

$(BIN)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) $? -o $@

$(EXE): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(EXE) $(BIN)/*.o

mkbin:
	mkdir -p bin
