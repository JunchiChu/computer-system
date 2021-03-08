#include <stdio.h>
#include <stdlib.h>

/*
 * Increments a number, passed by a pointer, and returns the same pointer
   passed to it after doing so.
 */
int* incr(int* n) {return ++n;}
int main(int argc, char** argv) {
    int n, *ret_addr;
    unsigned int i;

    if (argc == 1) {
        printf("error: usage: increment <arg1> <arg2> ... <argn>\n");
    }

    for (i = 1; i < argc; i++) {
        n = atoi(argv[i]);
        ret_addr = incr(&n);
        printf("%i ", *ret_addr);
    }
    printf("\n");

    return 0;
}
