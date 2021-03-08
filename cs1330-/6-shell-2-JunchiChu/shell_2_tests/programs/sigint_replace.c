#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void send_sigtstp(int sig) {
    kill(-getpid(), SIGTSTP);
}

int main(int argc, char **argv) {
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = &send_sigtstp;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGINT, &act, 0)) {
        perror("sigaction");
        exit(1);
    }

    const char *str = "kill me with SIGQUIT!\n";
    write(STDOUT_FILENO, str, strlen(str));
    while (1)
        pause();
}
