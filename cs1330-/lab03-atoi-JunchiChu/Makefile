
# Assembler
AS = as
AFLAGS = -g

# C Compiler
CC = gcc
CFLAGS = -g -Wall -std=gnu99

OBJECTS = my_atoi.o

EXECS = atoi atoi_validator

.PHONY: all atoi_validator atoi

all: $(EXECS)

$(OBJECTS): %.o: %.s
	$(AS) $(AFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(EXECS): %: $(OBJECTS) %.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rvf $(OBJECTS) $(EXECS)
