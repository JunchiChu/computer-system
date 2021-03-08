#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

/* appends stdout of the program at argv[2] to the file at argv[1] */
int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "error: usage: %s <file> <prog> [arg]*\n", argv[0]);
        exit(1);
    }

    close(STDOUT_FILENO);
    
    int fd = open(argv[1], O_CREAT | O_APPEND | O_WRONLY,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    assert(fd == STDOUT_FILENO);
    execvp(argv[2], argv + 2);
    perror("execv");
    return 1;
}

