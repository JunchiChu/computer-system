CC=gcc
CFLAGS = -std=gnu99 -Wall -fno-builtin -g
LDFLAGS=-lm
TESTS=run_tests

OBJS=parsing.o parsing_tests.o 
OBJS_ALL=$(OBJS)

all: $(TESTS)

%.o: %.c sec.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTS): $(OBJS_ALL)
	$(CC) $(CFLAGS) $(OBJS_ALL) -o $@ $(LDFLAGS)

clean:
	rm -f $(TESTS) $(OBJS)

