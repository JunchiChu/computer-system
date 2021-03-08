EXEC =		race
SRC = 		race.c
CFLAGS =    -Wall -pthread -std=c99 -D_GNU_SOURCE -g
CC =		/usr/bin/gcc

.PHONY: default
default: $(EXEC)

.PHONY: all
all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

.PHONY: clean
clean:
	rm -f *.o *~ $(EXEC)
