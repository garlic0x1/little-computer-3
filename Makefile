##
# VM
#
# @file
# @version 0.1

CC=gcc
CFLAGS=-g -Wall

PROGNAME=lc-3
INSTALLDIR=~/.local/bin
BINDIR=bin
BIN=$(BINDIR)/$(PROGNAME)

SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(BIN)
	cp $(BIN) $(INSTALLDIR)/$(PROGNAME)

clean:
	rm -r $(OBJ)/*
	rm -r $(BINDIR)/*

run: $(BIN)
	$(BIN)

# end
