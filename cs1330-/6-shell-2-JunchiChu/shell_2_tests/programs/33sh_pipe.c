#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE_SPLIT "-TO-"

int main(int argc, char **argv) {
    int i;
    for (i = 1; i < argc && strcmp(PIPE_SPLIT, argv[i]); i++);

    if (i <= 1 || argc - i <= 1) {
        fprintf(stderr, "error: usage %s <exec1> [arg]* %s <exec2> [arg]*\n",
                argv[0], PIPE_SPLIT);
        exit(1);
    }

    argv[i] = 0;

    int p[2];
    if (pipe(p) < 0) {
        perror("pipe");
        exit(2);
    }

    pid_t reader = fork();
    switch (reader) {
        case -1:
            perror("fork");
            exit(3);
        case 0:
            if (dup2(p[0], STDIN_FILENO) < 0) {
                perror("dup2");
                exit(4);
            }
            close(p[0]);
            close(p[1]);
            execvp(argv[i + 1], argv + i + 1);
            perror("execv");
            exit(5);
    }
    assert(reader);

    if (dup2(p[1], STDOUT_FILENO) < 0) {
        perror("dup2");
        kill(-reader, SIGKILL);
        exit(6);
    }
    close(p[0]);
    close(p[1]);

    execvp(argv[1], argv + 1);
    perror("execv");
    exit(5);

    return 0;
}
