CC = gcc
CFLAGS = -g -Wall

.PHONY: all

all: client 

client: client.o common.o
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf client