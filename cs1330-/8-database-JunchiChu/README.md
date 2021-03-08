# 8-database

STRUCTURE:

main:

follow by the handout, I have a signal handler, but before that, I ignore the SIGPIPE, and then call start_listener function. Next is to have a while loop which runs forever. And read input from the command, and will check if there is "s" "g" and "p" command.
if it is "s", will first lock the mutex and call control_stop, then unlock the mutex. The same thing happens with "g", but note that we need to broadcast after we change the condition variable. If it is "p", we need to check if there is a file name associated after the "p", else we just print it out. Anything else will print an invalid command warning.
After we finish up the DB we should delete all the threads, but before that use pthread_cond_wait to make sure no client is running. After that, we just clean up the database and cancel listener threads.

thread_cleanup:

We suppose to remove that client thread from the list, so lock both of the mutexes first and then check edge cases.
if the client ->next is NULL and if the client -> prev is NULL.

run client:

Just follow the handout, if the server is not acceptable then destruct the client and return NULL. else lock the mutex and add the thread into the list. After that pass in response and command into the while loop, which calls comm_serve. Since we might want to suspend the client if an "s" is received, so we call client_control_wait inside the while loop. And if a client needs to terminate, we must call pthread_cleanup_push to push the handler function into the stack.

client_control_wait/client_control_stop/client_control_release:

For the client_control_wait, we need to push the pthread_mutex_unlock function into the stack and have a while loop checking. If the stopped is 0, we just suspend the client-side, and only if we received stopped is not 0, we broadcast all the threads. so there is a stopped = 0 in stop function and a stopped++ in release function.

client_construct/destruct:

I just follow the handout, malloc and create a thread and detach a thread.
For the destruct just comm_shutdown and free the client.

delete_all:

Just follow the handout, check if thread_list is NULL, if not call pthread_cancel for each thread in the list. Make sure to lock and unlock the mutex.

sig_handler_constructor/monitor:
first malloc space for the sig struct and then just follow the slide, sigemptyset, and add SIGINT into the sig thread. and Mask off the SIGINT, and then create a monitor thread and detach that thread.

db.c:

My lock function: 
void locker(locktype lt, pthread_rwlock_t *lock) {
    int res;

    if (lt == l_write) {
        if ((res = pthread_rwlock_wrlock(lock))) {
            handle_error_en(res, "fail lock a");
        }
    } else {
        if ((res = pthread_rwlock_rdlock(lock))) {
            handle_error_en(res, "fail lock b");
        }
    }
}
Since sometimes we need a write lock and sometimes we need a read lock, so it is important to write a function that check which lock we need. And we pass this function into search. 

As far as I know: No bugs in my code so far. 