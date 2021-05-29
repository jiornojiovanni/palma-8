CC=gcc
CCFLAGS= -Wextra -Wall
LDFLAGS=-lncurses
DIRS=build
SRC=src
palmotto: $(SRC)/palmotto.c $(SRC)/interpreter.c
	$(CC) $(CCFLAGS) -o $(DIRS)/palmotto $(SRC)/palmotto.c $(SRC)/interpreter.c $(LDFLAGS)

debug: $(SRC)/palmotto.c $(SRC)/interpreter.c
	$(CC) $(CCFLAGS) -g -o $(DIRS)/palmotto-debug  $(SRC)/palmotto.c $(SRC)/interpreter.c $(LDFLAGS)

$(info $(shell mkdir -p $(DIRS)))