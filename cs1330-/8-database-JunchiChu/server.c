#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "./comm.h"
#include "./db.h"

/*
 * Use the variables in this struct to synchronize your main thread with client
 * threads. Note that all client threads must have terminated before you clean
 * up the database.
 */
typedef struct server_control {
    pthread_mutex_t server_mutex;
    pthread_cond_t server_cond;
    int num_client_threads;
} server_control_t;

/*
 * Controls when the clients in the client thread list should be stopped and
 * let go.
 */
typedef struct client_control {
    pthread_mutex_t go_mutex;
    pthread_cond_t go;
    int stopped;
} client_control_t;

/*
 * The encapsulation of a client thread, i.e., the thread that handles
 * commands from clients.
 */
typedef struct client {
    pthread_t thread;
    FILE *cxstr;  // File stream for input and output
    // For client list
    struct client *prev;
    struct client *next;
} client_t;

/*
 * The encapsulation of a thread that handles signals sent to the server.
 * When SIGINT is sent to the server all client threads should be destroyed.
 */
typedef struct sig_handler {
    sigset_t set;
    pthread_t thread;
} sig_handler_t;
client_control_t cc = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 1};
server_control_t sc = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0};
int flag_no_acc = 0;
client_t *thread_list_head;
pthread_mutex_t thread_list_mutex = PTHREAD_MUTEX_INITIALIZER;

void *run_client(void *arg);
void *monitor_signal(void *arg);
void thread_cleanup(void *arg);

/*
 * Called by client threads to wait until progress is permitted.
 */
void client_control_wait() {
    int res;
    if ((res = pthread_mutex_lock(&cc.go_mutex))) {
        handle_error_en(res, "have a issue in client control go mutex");
    }
    pthread_cleanup_push((void *)&pthread_mutex_unlock, (void *)&cc.go_mutex);
    while (!cc.stopped) {
        if ((res = pthread_cond_wait(&cc.go, &cc.go_mutex))) {
            handle_error_en(res, "have a issue in client control cond wait \n");
        }
    }
    pthread_cleanup_pop(1);
}
/*
 * Called by main thread to stop client threads
 */
void client_control_stop() { cc.stopped = 0; }
/*
 * Called by main thread to resume client threads
 */
void client_control_release() { cc.stopped++; }
/*
 * Called by listener (in comm.c) to create a new client thread
 */
void client_constructor(FILE *cxstr) {
    // You should create a new client_t struct here and initialize ALL
    // of its fields. Remember that these initializations should be
    // error-checked.
    //
    // TODO:
    // Step 1: Allocate memory for a new client and set its connection stream
    // to the input argument.
    // Step 2: Create the new client thread running the run_client routine.
    // Step 3: Detach the new client thread
    client_t *newclient;
    int res;
    // malloc a newclient
    if (!(newclient = malloc(sizeof(client_t)))) {
        perror("fail in mallocing");
        exit(1);
    }
    newclient->next = NULL;
    newclient->prev = NULL;
    newclient->cxstr = cxstr;
    // set the stream to be cxstr
    res = pthread_create(&newclient->thread, 0, run_client, newclient);
    if (res == 1) {
        handle_error_en(res, "cannot create a client thread");
    } else {
        res = pthread_detach(newclient->thread);
        if (res == 1) {
            handle_error_en(res, "cannot detach a client thread");
        } else {
            return;
        }
    }
}
// this is the client destructor, we pass in a client and just follow the
// handout we use comm shut down and then free the client
void client_destructor(client_t *client) {
    // use comm shutdown and free client as the handout suggested
    comm_shutdown(client->cxstr);
    free(client);
}

void *run_client(void *arg) {
    // TODO:
    // Step 1: Make sure that the server is still accepting clients.
    // Step 2: Add client to the client list and push thread_cleanup to remove
    //       it if the thread is canceled.
    // Step 3: Loop comm_serve (in comm.c) to receive commands and output
    //       responses. Execute commands using interpret_command (in db.c)
    // Step 4: When the client is done sending commands, exit the thread
    //       cleanly.
    //
    // Keep stop and go in mind when writing this function!
    int clear = 0;
    client_t *newclient = (client_t *)arg;

    if (flag_no_acc) {
        // global variable flag no acc to indicate that no more client is
        // requesting
        client_destructor(newclient);
        return NULL;
    }
    if (pthread_mutex_lock(&thread_list_mutex)) {
        perror("list mutex lock fail");
        exit(1);
    }
    // adding thread to the end of the list, this would cause a thread list
    // mutex deadlock issue
    client_t *cur = thread_list_head;
    if (!thread_list_head) {
        newclient->next = NULL;

        thread_list_head = newclient;
        thread_list_head->prev = NULL;

    } else {
        while (cur->next != NULL) {
            cur = cur->next;
        }

        cur->next = newclient;
        newclient->prev = cur;
    }
    // adding thread to the front of the list, this works well
    // if (thread_list_head) {
    //     newclient->next = thread_list_head;
    //     thread_list_head->prev = newclient;
    // }
    // thread_list_head = newclient;

    if (pthread_mutex_unlock(&thread_list_mutex)) {
        perror(" list mutex unlock fail");
        exit(1);
    }

    char respo[BUFLEN];
    char command[BUFLEN];
    respo[0] = clear;
    command[0] = clear;

    // lock the server mutex since we are changing the num of client threads
    if (pthread_mutex_lock(&sc.server_mutex)) {
        perror("mutex could not be locked: \n");
        exit(1);
    }
    // successfully add in one client should increment the num client
    sc.num_client_threads++;

    if (pthread_mutex_unlock(&sc.server_mutex)) {
        perror("mutex could not be locked: \n");
        exit(1);
    }
    // when the thread is finishing up, we should clean it
    pthread_cleanup_push(thread_cleanup, newclient);
    while (comm_serve(newclient->cxstr, respo, command) == 0) {
        // handle the s and g
        client_control_wait();
        interpret_command(command, respo, BUFLEN);
    }
    pthread_cleanup_pop(1);
    return NULL;
}
/*
 * Cancels all client threads. Must do this so that pthread_cleanup
 * is called and the appropriate cleaning up of resources is achieved
 * as desired.
 */

void delete_all() {
    // TODO: Cancel every thread in the client thread list with the
    // pthread_cancel function.
    int res;
    // lock the thread list mutex since are editing the list
    if ((res = pthread_mutex_lock(&thread_list_mutex))) {
        handle_error_en(res, "thread list mutex lock fail");
    }
    client_t *cur = thread_list_head;
    if (cur == NULL) {
        if ((res = pthread_mutex_unlock(&thread_list_mutex))) {
            handle_error_en(res, "thread list mutex unlock fail");
        }
        return;
    }
    // if cur thread exists, we cancel the thread
    while (cur != NULL) {
        pthread_cancel(cur->thread);
        client_t *nex = cur->next;

        cur = nex;
    }

    if ((res = pthread_mutex_unlock(&thread_list_mutex))) {
        handle_error_en(res, "thread list mutex unlock fail");
    }
}

/*
 * Cleanup routine for client threads, called on cancels and exit.
 */
void thread_cleanup(void *arg) {
    // TODO: Remove the client object from thread list and call
    // client_destructor. This function must be thread safe! The client must
    // be in the list before this routine is ever run.
    int res;

    if ((res = pthread_mutex_lock(&thread_list_mutex))) {
        handle_error_en(res, " thread clean up list mutex failure");
    }
    client_t *remove_client = (client_t *)arg;
    // edge case that if there is only one thread

    if (remove_client == thread_list_head) {
        thread_list_head->prev = NULL;
        thread_list_head->next = NULL;
        thread_list_head = NULL;

    } else {
        // if the next is null, handle the edge case
        if (remove_client->next == NULL && remove_client->prev != NULL) {
            client_t *pre = remove_client->prev;

            pre->next = NULL;
            // if the prev is null, handle the edge case
        } else if (remove_client->prev == NULL && remove_client->next != NULL) {
            client_t *nex = remove_client->next;
            nex->prev = NULL;

        } else {
            // if prev and next both exist, delete the middle element client
            client_t *pre = remove_client->prev;
            client_t *nex = remove_client->next;
            nex->prev = pre;
            pre->next = nex;
        }
    }
    //    if(remove_client->next){
    //        remove_client->next->prev = remove_client->prev;
    //    }
    //    if(remove_client==thread_list_head){
    //        thread_list_head=remove_client->next;
    //    }else{
    //        remove_client->prev->next= remove_client->next;
    //    }

    if ((res = pthread_mutex_unlock(&thread_list_mutex))) {
        handle_error_en(res, "fail unlock a mutex in cleanup ");
    }

    client_destructor(remove_client);
    if ((res = pthread_mutex_lock(&sc.server_mutex))) {
        handle_error_en(res, "fail at lock server control mutex \n");
    }
    sc.num_client_threads--;
    if (sc.num_client_threads == 0) {
        if (pthread_cond_signal(&sc.server_cond)) {
            perror("pthread_cond_broadcast failure: \n");
            exit(1);
        }
    }
    if ((res = pthread_mutex_unlock(&sc.server_mutex))) {
        handle_error_en(res, "mutex could not be locked: \n");
    }
}

/*
 * Code executed by the signal handler thread. Waits on SIGINT which is
 * in the sigset associated with it. Upon receiving SIGINT, deletes all
 * active clients.
 */
void *monitor_signal(void *arg) {
    // TODO: Wait for a SIGINT to be sent to the server process and cancel
    // all client threads when one arrives.
    sigset_t *pass_in_sig = (sigset_t *)arg;
    int sig = 0;
    int i = 5;
    int res;
    while (i != 0) {
        i++;
        res = sigwait(pass_in_sig, &sig);
        if (res) {
            perror("sig wait fails");
            exit(1);
        }
        if (sig == SIGINT) {
            printf("SIGINT received, cancelling all clients\n");
            delete_all();
        }
    }
    return NULL;
}
/*
 * Creates a thread who is solely responsible for waiting for and responding
 * to SIGINT. Before constructing this thread, makes sure that any other thread
 * ignores SIGINT upon receiving it.
 */
sig_handler_t *sig_handler_constructor() {
    int res;
    // first malloc for the space for sig handler
    sig_handler_t *sig_thread;
    if (!(sig_thread = malloc(sizeof(sig_handler_t)))) {
        perror("malloc failed: \n");
        exit(1);
    }
    // empty the set first
    res = sigemptyset(&sig_thread->set);
    if (res != 0) {
        perror("sigemptyset error");
        exit(1);
    }
    // add sig int to the set
    res = sigaddset(&sig_thread->set, SIGINT);
    if (res != 0) {
        perror("sig add set error");
        exit(1);
    }
    // block that sigint signal in the server main
    if ((res = pthread_sigmask(SIG_BLOCK, &sig_thread->set, 0))) {
        handle_error_en(res, "fail sigmask sig thread");
    }
    // create a monitor thread to take care of sigint and detach the thread
    res = pthread_create(&sig_thread->thread, 0, monitor_signal,
                         &sig_thread->set);

    if (res != 0) {
        handle_error_en(res, "fail create sig thread");
    } else {
        if ((res = pthread_detach(sig_thread->thread))) {
            handle_error_en(res, "fail detach sig thread");
        }
    }

    return sig_thread;
}
/*
 * Cancels signal handling thread and frees space allocated
 * to the signal handling struct to avoid memory corruption.
 */
void sig_handler_destructor(sig_handler_t *sighandler) {
    int res;
    // just cancel a thread and doing the error checking
    // free the singal handler
    res = pthread_cancel(sighandler->thread);
    if (res == 1) {
        handle_error_en(res, "sig handler thread cancel failure");
    } else {
        free(sighandler);
    }
}
/*
 * Main of program.
 */
int main(int argc, char *argv[]) {
    // TODO:
    // Step 1: Set up the signal handler.
    // Step 2: block SIGPIPE so that the server does not abort when a client
    // disocnnects Step 3: Start a listener thread for clients (see
    // start_listener in
    //       comm.c).
    // Step 4: Loop for command line input and handle accordingly until EOF.
    // Step 5: Destroy the signal handler, delete all clients, cleanup the
    //       database, cancel and join with the listener thread
    //
    // You should ensure that the thread list is empty before cleaning up the
    // database and canceling the listener thread. Think carefully about what
    // happens in a call to delete_all() and ensure that there is no way for a
    // thread to add itself to the thread list after the server's final
    // delete_all().
    // printf("dem %s dem %s dem %s",*argv,*(argv+1),argv[2]);

    int i = 5;
    int res;
    // handle the sigpipe
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("sigpipe fail");
        exit(1);
    }
    // construct the sig handler
    sig_handler_t *sigh = sig_handler_constructor();
    // create a listen thread
    pthread_t listen = start_listener(atoi(*(argv + 1)), &client_constructor);
    while (i != 0) {
        ssize_t checkread = 0;
        char input[500];
        char *tokenn;
        char *tokens[512];
        int clean = 0;
        // read the input from the command line
        checkread = read(STDIN_FILENO, input, 500);
        while (clean != 512) {
            tokens[clean] = NULL;
            clean++;
        }
        int idxs = 0;

        while (idxs != 512) {
            if (input[idxs] == '\t') {
                input[idxs] = ' ';
            }
            idxs++;
        }
        tokenn = strtok(input, " ");

        int i = 0;
        while (tokenn != NULL) {
            tokens[i] = tokenn;

            i++;
            tokenn = strtok(NULL, " ");
        }
        // we should only concern about s g p, in order to avoid trash chars,
        // terminate the string
        if (input[0] == 's' || input[0] == 'g' || input[0] == 'p') {
            tokens[0][1] = '\0';
        }

        if (checkread != 0) {
            // if there is s in the string, just change the variable stopped in
            // client control stop
            if (strcmp(tokens[0], "s") == 0) {
                if ((res = pthread_mutex_lock(&cc.go_mutex))) {
                    handle_error_en(res, "s lock mutex fail");
                }
                fprintf(stderr, "stopping all cilents\n");
                client_control_stop();
                if ((res = pthread_mutex_unlock(&cc.go_mutex))) {
                    handle_error_en(res, "s unlock fail ");
                }
            } else if (strcmp(tokens[0], "g") == 0) {
                if ((res = pthread_mutex_lock(&cc.go_mutex))) {
                    handle_error_en(res, "g lock fail");
                }
                fprintf(stderr, "releasing all clients\n");
                // this time it is important to broadcast, otherwise thread
                // won't release
                if ((res = pthread_cond_broadcast(&cc.go))) {
                    handle_error_en(res, "pthread_cond_broadcast fail");
                }
                client_control_release();
                if ((res = pthread_mutex_unlock(&cc.go_mutex))) {
                    handle_error_en(res, "g unlock fail");
                }

            } else if (strcmp(tokens[0], "p") == 0) {
                fprintf(stderr, "printing the db\n");
                int char_index = 0;
                // this while loop is to ensure we parse the filename
                while (char_index < 512) {
                    if (tokens[1] == NULL) {
                        break;
                    }
                    if (tokens[1][char_index] == '.') {
                        tokens[1][char_index + 4] = '\0';
                        break;
                    }
                    char_index++;
                }
                // to check if there is a file name afte the p, if there is, you
                // want to do db_print(filename)
                if (tokens[1] == NULL) {
                    db_print(NULL);
                } else {
                    db_print(tokens[1]);
                }

            } else {
                fprintf(stderr, "invalid command input, please reinsert\n");
            }

        } else {
            // receive EOF, it is time to exit db
            if (fprintf(stdout, "exiting database\n") < 0) {
                perror("fprintf failure: \n");
                exit(1);
            }
            flag_no_acc = 1;
            delete_all();
            if ((res = pthread_mutex_lock(&sc.server_mutex))) {
                handle_error_en(res, "lock sc fail");
            }
            // we need a while loop to check if there is still client threads
            // running, if there is we must wait We should boardcast the server
            // in thread_cleanup when the threads are gone, so it won't suspend
            // forever
            while (sc.num_client_threads > 0) {
                if ((res = pthread_cond_wait(&sc.server_cond,
                                             &sc.server_mutex))) {
                    handle_error_en(res, "server cond wait fail");
                }
            }
            if ((res = pthread_mutex_unlock(&sc.server_mutex))) {
                handle_error_en(res, "unlock sc fail");
            }
            db_cleanup();
            // cleanup the listen thread and the signal thread
            if ((res = pthread_cancel(listen))) {
                handle_error_en(res, "cancel server thread");
            }
            if ((res = pthread_cancel(sigh->thread))) {
                handle_error_en(res, "cancel server thread");
            }
            sig_handler_destructor(sigh);

            exit(0);
        }
    }
    return 0;
}