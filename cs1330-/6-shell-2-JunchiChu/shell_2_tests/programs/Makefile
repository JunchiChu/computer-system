# Makefile for single-C-file programs.  Add new targets to EXECS

CC = gcc
CFLAGS = -std=c99 -D_GNU_SOURCE -Wall
RMFLAGS = -rvf

EXECS = myint mystop mysplit myspin mypid
EXECS += delayed_echo segfault mixup exit_status delay check_process_state
EXECS += 33sh_to_file 33sh_append_file 33sh_pipe
EXECS += sigint_n sigcont_n sigtstp_n sigstop_n
EXECS += sigcont_ignore sigint_ignore
EXECS += sigint_replace sigtstp_replace

.PHONY: all
all : $(EXECS)

$(EXECS) : % : %.c
	$(CC) $(CFLAGS) $^ -o $@
	chmod o+x $@

.PHONY: clean
clean:
	rm $(RMFLAGS) $(EXECS)
