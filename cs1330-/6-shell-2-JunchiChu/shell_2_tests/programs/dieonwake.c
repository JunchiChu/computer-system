/* This program executes a count from 1 to 10 with a 1 second delay
 * for each number.
 *
 * The program kills itself when it receives SIGCONT.
 */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include "33sh.h"

void exit_hand(int sig) {
	write(STDERR_FILENO, "Exit\n", 5);
	raise(SIGKILL);
}

int main(int argc, char** argv) {
	//Exit on SIGCONT
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = exit_hand;
	sigaction(SIGCONT, &act, NULL);

	printf("Waiting\n");
	int i;
	for(i = 1; i<=10; i++) {
		if(robust_wait(1) == -1)
			return -1;
		printf("%d\n", i);
	}
	
	printf("Done\n");
	return 0;
}
