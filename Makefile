CC=gcc
LDFLAGS=-lncurses
DIRS=build
SRC=src
palmotto: $(SRC)/palmotto.c $(SRC)/interpreter.c
	$(CC) -o $(DIRS)/palmotto $(SRC)/palmotto.c $(SRC)/interpreter.c $(LDFLAGS)

debug: $(SRC)/palmotto.c $(SRC)/interpreter.c
	$(CC) -g -o $(DIRS)/palmotto-debug  $(SRC)/palmotto.c $(SRC)/interpreter.c $(LDFLAGS)

$(info $(shell mkdir -p $(DIRS)))