/* 
 * myint.c - Another handy routine for testing your tiny shell
 * 
 * usage: myint <n>
 * Sleeps for <n> seconds and sends SIGINT to itself.
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
        fprintf(stderr, "error: usage: %s <n>\n", argv[0]);
        exit(0);
    }
    robust_wait(atoi(argv[1]));
	
    pid_t pid = getpid(); 

    if (kill(pid, SIGINT) < 0)
        fprintf(stderr, "kill (int) error");

    return 0;
}
