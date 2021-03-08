CC = gcc
GEN = generator
SOL = solver
SOL_FULL = solver_full
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -g

GEN_HEADERS = common.h generator.h
GEN_OBJS = generator.c common.c

SOL_HEADERS = common.h solver.h
SOL_OBJS = solver.c common.c

EXECS = $(GEN) $(SOL) $(SOL_FULL) 

all: $(EXECS)

$(GEN): $(GEN_HEADERS) $(GEN_OBJS)
	$(CC) $(CFLAGS) -o $(GEN) $(GEN_OBJS)


$(SOL): $(SOL_HEADERS) $(SOL_OBJS)
	$(CC) $(CFLAGS) -o $(SOL) $(SOL_OBJS)
	

$(SOL_FULL): $(SOL_OBJS) $(SOL_HEADERS)
	$(CC) $(CFLAGS) -o $(SOL_FULL) -DFULL $(SOL_OBJS)


clean:
	rm -f $(EXECS)
