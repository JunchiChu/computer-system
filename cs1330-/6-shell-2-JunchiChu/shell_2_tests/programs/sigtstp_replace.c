#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void send_sigint(int sig) {
    kill(-getpid(), SIGINT);
}

int main(int argc, char **argv) {
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = &send_sigint;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGTSTP, &act, 0)) {
        perror("sigaction");
        exit(1);
    }

    const char *str = "Don't TSTP me I'll die :(\n";
    write(STDOUT_FILENO, str, strlen(str));
    while (1)
        pause();
}
