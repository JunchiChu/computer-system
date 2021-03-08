#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include "33sh.h"

void ignore(int sig) {
	assert(sig || 1);
    WRITE(STDOUT_FILENO, "Received SIGINT.\n");
}

int main() {
	struct sigaction sig;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_handler = ignore;
	
	sigaction(SIGINT, &sig, NULL);
	
	WRITE(STDOUT_FILENO, "This program ignores SIGINT.\n\n");
	WRITE(STDOUT_FILENO,
            "It loops forever, printing a message each time "
            "SIGINT is received.\n\n");
	WRITE(STDOUT_FILENO,
            "SIGINT should not remove this program from the jobs list.\n\n");
	
	WRITE(STDOUT_FILENO, "Looping.\n");
	while(1);

	return 0;
}
