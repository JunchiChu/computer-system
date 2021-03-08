# 6-shell-2

I add new features which handles the singals compare to shell1.
Here is my newly helper functions

void check_zombie (job_list_t *myjoblist)

This function is checking for zombie state, just follow up the lecture slides, we put this function before the prompt was printed. If the program exit normally, we just print terminated. If there is a signal to terminate the program, we print terminate and also remove that job from the list...If the job was stopped, we need to update the job state, changing it to stopped. And if a sigcont was
received, we will just say its resumed.

int end_of_file_check(ssize_t checkread, job_list_t *myjoblist)



This helper function is to check whether there is a EOF at the very end or the
user hit control d to terminate the program. If that is the case, we should just
cleanup the job list. This helper return 1 when we need to exit, 0 otherwise.

int check_build_in(char *tokens[512])


This helper function takes token string as input, as will check if there are
build in commands. The return value is 0, if there is no build in command, and
return 5 if there is a build in command

How I handle singals:

If the program is running in shell, set signal to be ignored, otherwise if it is in fg, we want to set it to default. So use tcsetpgrp to switch signals when we need it. If it is a bg process, we set it back so the signal can be ignored.

How I implement bg/fg:

I implement this before the fork, so it is kind of be treated as a build in command. If it is bg, we just rearrange the job list and error checking. If it is a fg, the situation is different. We need to bring the bg job to fg job, so we called a waitpid since the job now is fg job, get the group id of that job, and checking those singals again. When we done, call tcsetpgrp change the control back to shell.

In fork:

After we fork, I decide to change signal in fork, and call execv inside the fork.

After fork:

It depends on the process, if it is a bg process, we skip the waipid. If it is a fg process, we cannot skip the waitpid. For any bg process, we reflect the update in our job list. After we waitpid, we should check the status of waitpid. This is similar as how we check the zombie state. If the program exit normall\
y, we just print terminated. If there is a signal to terminate the program, we print terminate and also remove that job from the list...If the job was stopped, we need to update the job state, changing it to stopped. And if a sigcont was
received, we will just say its resumed.
