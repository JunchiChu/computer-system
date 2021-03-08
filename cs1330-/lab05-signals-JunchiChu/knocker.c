/* This program reads command line arguments
 * and sends the corresponding signals to the specified PID */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
/* Main function */
int main(int argc, char** argv) {
  // TODO: Parse command line arguments and send the specified signals
  // Pause briefly between signals using sleep(), usleep(), or nanosleep()
  printf("--%s\n",argv[1]);
  pid_t pid;
  // pid  = getpid();
  //pid = 12;
  //  printf("pid%d\n",pid);
  pid = atoi(argv[1]);
  //pid = 4304;
   printf("pid%d\n",pid);

    int c =0;
  // printf("%c",argv[2][0]);
    while(argv[2][c]!='\0'){

      
    if(argv[2][c]=='c'){
      sleep(1);
      kill(pid,2);
      //sleep(1);
    }else if(argv[2][c]=='z'){
      sleep(1);
      kill(pid,20);
      //sleep(1);

    }else {
      sleep(1);
      kill(pid,3);
      //sleep(1);
       
    }
    c++;
    }
    //printf("pid is %ld\n",strlen(argv[2]));
  
}
