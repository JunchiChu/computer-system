CC = gcc
CFLAGS = -g3 -Wall -Wextra -Wconversion -Wcast-qual -Wcast-align -g
CFLAGS += -Winline -Wfloat-equal -Wnested-externs
CFLAGS += -pedantic -std=gnu99 -Werror
EXECS= 33sh 33noprompt
PROMPT = -DPROMPT

.PHONY: all clean

all: $(EXECS)
33sh: sh.c
	$(CC) $(CFLAGS) -o 33sh -DPROMPT sh.c jobs.c jobs.h
33noprompt: sh.c
	$(CC) $(CFLAGS) -o 33noprompt sh.c jobs.c jobs.h
clean:
	rm -f $(EXECS)

