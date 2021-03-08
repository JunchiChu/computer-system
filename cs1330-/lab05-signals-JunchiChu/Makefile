CC = gcc
CFLAGS = -Wall -std=gnu99
EXECS = siglab knocker


all: $(EXECS)

siglab: siglab.c
	$(CC) $(CFLAGS) $< -o $@

knocker: knocker.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECS)
