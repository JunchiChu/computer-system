# Flags to pass to compiler
CC = gcc
CFLAGS = -Wall -O2 -std=c99 -D_GNU_SOURCE -ggdb
EXECS = copy quiet_repl loud_repl test
HEADERS = lower.h upper.h
CPF =  cp ./files/lower.c ./
CPF2 = cp ./files/lower.h ./

.PHONY: all clean

all : $(EXECS)

copy:
	cp ./files/lower.c ./
	cp ./files/lower.h ./


quiet_repl: $(HEADERS) repl.c lower.c upper.c 
	$(CC) $(CFLAGS) -o quiet_repl repl.c lower.c upper.c

loud_repl: $(HEADERS) repl.c lower.c upper.c
	$(CC) $(CFLAGS) -o loud_repl -DEXTRA_LOUD repl.c lower.c upper.c

test: 
	./repltests quiet_repl loud_repl

clean:
	rm -f $(EXECS)
	rm -f ./lower.c
	rm -f ./lower.h
# TODO: Create rest of Makefile
