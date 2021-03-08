/* 
 * mysplit.c - Another handy routine for testing your shell
 * 
 * usage: mysplit <n>
 * Fork a child that spins for <n> seconds
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
    if (fork() == 0) { /* child */
        robust_wait(atoi(argv[1]));
        exit(0);
    }

    /* parent waits for child to terminate */
    wait(NULL);

    return 0;
}
