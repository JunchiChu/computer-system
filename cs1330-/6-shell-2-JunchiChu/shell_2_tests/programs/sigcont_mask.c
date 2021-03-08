#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char **argv) {
    sigset_t cont;
    sigemptyset(&cont);
    sigaddset(&cont, SIGCONT);

    if (sigprocmask(SIG_BLOCK, &cont, 0)) {
        perror("sigprocmask");
        exit(1);
    }

    printf("SIGCONT blocked\n");
    while (1) {
        pause();
        printf("Received signal\n");
    }

    return 0;
}
