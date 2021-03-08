#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "33sh.h"

#define BUFSIZE 16
void read_states(FILE *f)
{
    char status;
    pid_t id;
    char buf[BUFSIZE];
    while (fgets(buf, BUFSIZE, f))
    {
        if (sscanf(buf, "%d", &id) < 1)
            continue;

        sprintf(buf, "/proc/%d/stat", id);
        FILE *stat = fopen(buf, "r");
        if (!stat)
        {
            sprintf(buf, "(%d): does not exist\n", id);
            WRITE(STDOUT_FILENO, buf);
            continue;
        }
        else if (!stat || fscanf(stat, "%d %s %c", &id, buf, &status) < 3)
        {
            sprintf(buf, "(%d): error parsing state\n", id);
            WRITE(STDOUT_FILENO, buf);
            continue;
        }
        fclose(stat);

        const char *state;
        switch (status)
        {
        case 'R':
            state = "running";
            break;
        case 'D':
            state = "blocking";
            break;
        case 'S':
            state = "sleeping";
            break;
        case 'Z':
            state = "zombie";
            break;
        case 'T':
            state = "suspended";
            break;
        default:
            state = "unknown state";
            break;
        }
        sprintf(buf, "(%d): %s\n", id, state);
        WRITE(STDOUT_FILENO, buf);
    }
}

int main(int argc, char **argv)
{
    int i;
    for (i = 1; i < argc; i++)
    {
        FILE *f = fopen(argv[i], "r");
        if (f)
        {
            read_states(f);
        }
        else
        {
            fprintf(stderr, "could not open %s\n", argv[i]);
            perror("fopen");
        }
    }

    if (i == 1)
        read_states(stdin);

    return 0;
}
