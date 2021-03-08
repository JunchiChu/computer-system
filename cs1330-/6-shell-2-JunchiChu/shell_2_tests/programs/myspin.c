/* 
 * myspin.c - A handy program for testing your tiny shell 
 * 
 * usage: myspin <n>
 * Sleeps for <n> seconds
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "33sh.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Given: %d arguments" , argc);
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        exit(0);
    }

    robust_wait(atoi(argv[1]));
    return 0;
}
