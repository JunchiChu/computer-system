#ifndef RED_SHELL_H
#define RED_SHELL_H

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define WRITE(__fd, __str) write(__fd, __str, strlen(__str))

int robust_wait(int sec) {
	struct timeval now;
	gettimeofday(&now, NULL);
	struct timeval later = now;
	later.tv_sec += sec;

	while(1) {
		if(timercmp(&now, &later, <)) {
			struct timeval time_left;
			timersub(&later, &now, &time_left);
			
			struct timespec wait_time;
			wait_time.tv_sec = time_left.tv_sec;
			wait_time.tv_nsec = 1000 * time_left.tv_usec;

			if(nanosleep(&wait_time, NULL) == -1 && errno != EINTR) {
				perror("nanosleep");
				return 1;
			} else {
				gettimeofday(&now, NULL);
			}
		} else {
			return 0;
		}
	}
}

#endif
