/* This program executes a count from 1 to 10 with a 1 second delay
 * for each number.
 *
 * The program catches SIGINT, SIGTSTP, and SIGQUIT and prints a message
 * for each one, instead of the default behavior.
 */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include "33sh.h"

void sigint_hand(int sig) {
	write(STDERR_FILENO, "SIGINT\n", 7);
}

void sigtstp_hand(int sig) {
	write(STDERR_FILENO, "SIGTSTP\n", 8);
}

void sigquit_hand(int sig) {
	write(STDERR_FILENO, "SIGQUIT\n", 8);
}

int main(int argc, char** argv) {
	// Install handlers that print the signal name and do nothing else
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = sigint_hand;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = sigtstp_hand;
	sigaction(SIGTSTP, &act, NULL);
	act.sa_handler = sigquit_hand;
	sigaction(SIGQUIT, &act, NULL);

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
