/* 
 * mystop.c - Another handy routine for testing your tiny shell
 * 
 * usage: mystop <n>
 * Sleeps for <n> seconds and sends SIGTSTP to itself.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "33sh.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        exit(0);
    }
    robust_wait(atoi(argv[1]));
	
    pid_t pid = getpid(); 

    if (kill(-pid, SIGTSTP) < 0)
        fprintf(stderr, "kill (tstp) error");

    return 0;
}
