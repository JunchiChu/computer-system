#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "33sh.h"

int main(int argc, char **argv) {

    int nsleep = atoi(argv[1]);

    pid_t parent = getpid();
    pid_t pid = fork();

    if (!pid) {
        while (1) {
            robust_wait(nsleep);
            kill(parent, SIGCONT);
        }

        exit(0);
    }
    else if (pid < 0) {
        perror("fork");
        exit(1);
    }

    kill(parent, SIGTSTP);
    kill(pid, SIGINT);

    robust_wait(nsleep);

    return 0;
}
