#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "33sh.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "error: usage: %s <delay> <prog> [<arg>*]\n",
                argv[0]);
        exit(127);
    }

    robust_wait(atoi(argv[1]));

    char buf[128];
    sprintf(buf, "Executing %s\n", argv[2]);
    WRITE(STDOUT_FILENO, buf);

    execvp(argv[2], argv + 2);
    perror("execv");
    return 126;
}
