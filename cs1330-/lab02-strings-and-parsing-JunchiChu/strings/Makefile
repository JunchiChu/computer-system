CC=gcc
CFLAGS = -std=gnu99 -Wall -fno-builtin -g
LDFLAGS=-lm
TESTS=run_tests

OBJS=strings.o strings_tests.o
OBJS_ALL=strings_unit_tests.o $(OBJS)

all: $(TESTS)

%.o: %.c sec.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTS): $(OBJS_ALL)
	$(CC) $(CFLAGS) $(OBJS_ALL) -o $@ $(LDFLAGS)

clean:
	rm -f $(TESTS) $(OBJS)