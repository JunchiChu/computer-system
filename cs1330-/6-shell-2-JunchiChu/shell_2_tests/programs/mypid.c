#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "33sh.h"

/* simple program that writes its pid to stdout and exits */
int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "error: usage: %s [<delay>]\n", argv[0]);
        exit(1);
    }

    char buf[128];
    sprintf(buf, "%d\n", getpid());

    WRITE(STDOUT_FILENO, buf);
    close(STDOUT_FILENO); /* flush if connected to a pipe or file */

    if (argc > 1)
        robust_wait(atoi(argv[1]));

    return 0;
}
