#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include "33sh.h"

int cont;

void ignore(int sig) {
	assert(sig || 1);
	cont = 1;
	WRITE(STDOUT_FILENO, "Received SIGCONT.\n");
	kill(0, SIGSTOP);
}

void sig_exit(int sig) {
	write(STDIN_FILENO, "Exiting.\n", strlen("Exiting.\n"));
	exit(0);
}


int main() {
	cont = 0;
	
	struct sigaction sig;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_handler = ignore;
	sigaction(SIGCONT, &sig, NULL);
	
	sig.sa_handler = sig_exit;
	sigaction(SIGINT, &sig, NULL);
	
	WRITE(STDOUT_FILENO, "This program loops until it receives a SIGTSTP or other stop signal.\n\n");
	WRITE(STDOUT_FILENO, "It has a signal handler for SIGCONT that re-sends a SIGTSTP,\n"
		"so that it can never actually be resumed once stopped.\n\n");
	WRITE(STDOUT_FILENO, "Use it to test that processes are moved into the foreground correctly.\n\n");
	WRITE(STDOUT_FILENO, "Looping.\n");
	while (1) {
		while (!cont);
		WRITE(STDOUT_FILENO, "Received SIGCONT.\n");
	}
	
	return 0;
}
