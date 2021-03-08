#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "33sh.h"

int main(int argc, char **argv) {

	if (argc < 3) {
		fprintf(stderr, "error: usage: %s <delay> <msg> ...\n", argv[0]);
		exit(1);
	}

    robust_wait(atoi(argv[1]));
	
	argv[1] = "/bin/echo";
	execv(argv[1], argv + 1);
	
	perror("exec echo");
	
	return 1;
}
