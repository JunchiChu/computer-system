#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Forward Declarations
int install_handler(int sig, void (*handler)(int));
void sigint_handler(int sig);
void sigtstp_handler(int sig);
void sigquit_handler(int sig);
int read_and_echo();
time_t c;
time_t z;
time_t q;
int count=0;
/* main
 * Install the necessary signal handlers, then call read_and_echo().
 */
int main(int argc, char** argv) {
  sigset_t old;
  sigset_t full;
  sigfillset(&full);

  // Ignore signals while installing handlers
  sigprocmask(SIG_SETMASK, &full, &old);

  // Install signal handlers
  if (install_handler(SIGINT, &sigint_handler))
    perror("Warning: could not install handler for SIGINT");

  if (install_handler(SIGTSTP, &sigtstp_handler))
    perror("Warning: could not install handler for SIGTSTP");

  if (install_handler(SIGQUIT, &sigquit_handler))
    perror("Warning: could not install handler for SIGQUIT");

  // Restore signal mask to previous value
  sigprocmask(SIG_SETMASK, &old, NULL);

  read_and_echo();

  return 0;
}

/* install_handler
 * Installs a signal handler for the given signal
 * Returns 0 on success, -1 on error
 */

void handlerrr(int signo) {

  printf("I received signal %d." "Whoopee!!\n",signo);
}
int install_handler(int sig, void (*handler)(int)) {
  // TODO: Use sigaction() to install a the given function
  // as a handler for the given signal.
  struct sigaction act;
  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags=0;
  int res=sigaction(sig,&act,NULL);
  if(res == -1){
    return -1;
  }else{
  return 0;
  }
  return 0;
}

/* sigint_handler
 * Respond to SIGINT signal (CTRL-C)
 *
 * Argument: int sig - the integer code representing this signal
 */
void sigint_handler(int sig) {
  c=time(NULL);
  count=count+1;
  printf("c %ld\n",c);
   printf("I received signal int %d." "Whoopee!!\n",sig);
   
}

/* sigtstp_handler
 * Respond to SIGTSTP signal (CTRL-Z)
 *
 * Argument: int sig - the integer code representing this signal
 */
void sigtstp_handler(int sig) {
  //signal(sig,);
  z=time(NULL);
  count=count+2;
  printf("z %ld\n",z);

   printf("I received signal stop %d." "Whoopee!!\n",sig);

}

/* sigquit_handler
 * Catches SIGQUIT signal (CTRL-\)
 *
 * Argument: int sig - the integer code representing this signal
 */
void sigquit_handler(int sig) {
  q=time(NULL);
  count=count+3;
   printf("I received signal quit%d." "Whoopee!!\n",sig);
     printf("c z q count %ld %ld %ld %d\n",c,z,q,count);
     //exit(1);
     if( count==6){
   exit(1);
    }
}

/* read_and_echo
 * Read input from stdin, echo to stdout.
 * Return 0 on EOF, -1 on error
 */
int read_and_echo() {
  // TODO: Read from stdin and write to stdout


  // Use the async-signal-safe syscalls read() and
  int i = 120;
  int checkread;
  //int checkwrite;
  while(i != 0 ){
  char input[1024];
  checkread  = read(STDIN_FILENO,input,1024);
  if(checkread ==0){

    return 0;
  }
  //  if(checkread == -1){
  // perror("read fail");
  //  return -1;
  // }
  //input[checkread -1 ]='\n';
  input[checkread-1]='\0';
  
  write(STDOUT_FILENO,input,checkread-1);
  printf("\n");
  //if(checkwrite == -1){
  //  perror("write fail");
  //  return -1;
  // }

  //i--;
  }
  return 0;
}
