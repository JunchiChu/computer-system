CC = gcc
CFLAGS = -Wall -O0 -g -std=c99

EXECS = tester

.PHONY: all
all: $(EXECS)

tester: string_fns.o linkedlist.o tester.c
	$(CC) $(CFLAGS) $^ -o $@

string_fns.o: string_fns.c string_fns.h
	$(CC) $(CFLAGS) -c string_fns.c

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c

.PHONY: clean
clean:
	rm -rvf *.o $(EXECS)
