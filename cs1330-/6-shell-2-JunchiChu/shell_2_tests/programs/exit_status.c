#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "33sh.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "error: usage: %s <delay> <status>\n", argv[0]);
        exit(1);
    }

    robust_wait(atoi(argv[1]));
    return atoi(argv[2]);
}
