#include <fcntl.h>  // for open
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>  // for close
#include <unistd.h>
#include "jobs.h"
#define MAXSIZE 1024
#define SIZE 512
job_list_t *myjoblist;
pid_t shellid;
int job = 1;
void check_zombie(job_list_t *m);
int end_of_file_check(ssize_t m, job_list_t *k);
int check_build_in(char *tokens[512]);
int main() {
    int i = 10;
    i = 10;
    shellid = getpgid(getpid());
    job_list_t *myjoblist;
    myjoblist = init_job_list();

    signal(SIGINT, SIG_IGN);  // for shell command we ignore all these signals
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    while (i != 0) {
        pid_t child;
        char input[MAXSIZE];
        ssize_t checkread;

        char *tokenn;
        char *arg;
        char *art;
        int fd = 0;
        int fg_bg = 0;   // if there is & in the command fg_bg is 1, 0 otherwise
        int fg_com = 0;  // if there is a fg command
        int bg_com = 0;  // if there is a bg command

        check_zombie(myjoblist);
#ifdef PROMPT
        if (printf("33sh>") < 0) {
            perror("prompt error");
        }

        if (fflush(stdout) < 0) {
            perror("fflush error");
        }
#endif
        checkread = read(STDIN_FILENO, input, MAXSIZE);

        if (checkread < 0) {
            perror("read fail");
        }
        if (end_of_file_check(checkread, myjoblist) == 1) {
            exit(0);
        }

        if (checkread != 0) {
            input[checkread - 1] = '\0';

            if (checkread != 1) {
                char *tokens[512];
                char *argv[512];
                int clean = 0;

                while (clean != 512) {
                    tokens[clean] =
                        NULL;  // clean up tokens and argv every iteration
                    argv[clean] = NULL;  // hence the previous input won't mess
                                         // up the current input
                    clean++;
                }
                char cinput[MAXSIZE];
                int idxs = 0;

                while (idxs != 512) {
                    if (input[idxs] == '\t') {
                        input[idxs] = ' ';
                    }
                    idxs++;
                }

                if (strcpy(cinput, input) == NULL) {
                    fprintf(stderr, "string copy failed!");
                    cleanup_job_list(myjoblist);

                    exit(0);
                }

                tokenn = strtok(input, " ");
                if (tokenn != NULL) {
                    int i = 0;
                    while (tokenn != NULL) {
                        tokens[i] = tokenn;  // we split the input string by
                                             // using strtok

                        i++;
                        tokenn = strtok(NULL, " ");
                    }

                    int j = 0;
                    int double_input_redirect = 0;
                    while (tokens[j] != NULL) {
                        if (strcmp(tokens[j], "<") == 0) {
                            double_input_redirect++;  // detect double input
                                                      // redirect violation.
                        }

                        if (strcmp(tokens[0], ">") == 0 ||
                            strcmp(tokens[0], "<") == 0 ||
                            strcmp(tokens[0], ">>") == 0) {
                            while (tokens[j + 2] != NULL) {
                                argv[j] =
                                    tokens[j + 2];  // checking edge cases when
                                                    // the first input command
                                                    // is redirection
                                j++;
                            }
                            break;
                        }

                        argv[j] = tokens[j];

                        j++;
                    }

                    if (double_input_redirect >= 2) {
                        fprintf(stderr, "cann't have multipe directions input");
                        exit(0);
                    }
                    char *ret;

                    ret = strstr(cinput, "/");
                    /*
                    We tokenize the input, note that the difference between
tokens and argvs is the first token. So we use strstr to get rid of all extra
path "/" until we get the correct argv[0] The purpose of this if while structure
is to get the correct argv [0]
                     */

                    if (ret != NULL && *ret == '/') {
                        arg = ret;
                        art = ret;
                        while (strcspn(arg, "/") == 0) {
                            arg++;
                            art = strchr(arg, '/');
                            if (art == NULL) {
                                break;
                            }
                            arg = strchr(arg, '/');
                        }
                        argv[0] = arg;
                        argv[0] = strtok(argv[0], " ");
                    }

                    if (strcmp(tokens[0], "exit") == 0) {
                        cleanup_job_list(myjoblist);
                        exit(0);  // if the user want to exit, we should exit
                                  // properly.
                    }

                    /*
build in is a flag to indicate whether it is the case we should run our command
in build in way or we should call execv to run the command.
                     */

                    int build_in = 0;

                    /*
           if build in is 0 that indicates we should use fork to create a child
process Then we need to run execv to process our command!
                     */

                    build_in = check_build_in(tokens);

                    fg_com = 0;
                    bg_com = 0;
                    if (strcmp(tokens[0], "fg") == 0) {
                        fg_com = 1;
                    } else if (strcmp(tokens[0], "bg") == 0) {
                        bg_com = 1;
                    } else {
                    }
		    /*
If there is fg in the input command, we should call another waitpid, and do some error checking make sure the pid is valid and job exist. Doing the same process just like the repl.
		     */
                    if (fg_com == 1) {
                        int job_id = (int)(argv[1][1]) - 48;

                        pid_t fg_pid = get_job_pid(myjoblist, job_id);

                        if (fg_pid < 0) {
                            fprintf(stderr, "job not found\n");
                        }

                        if (kill(-fg_pid, SIGCONT) < 0) {
                            fprintf(stderr, "job not found\n");
                        }
                        int sig = 0;

                        if (job_id > 0 && fg_pid >= 0) {
                            update_job_jid(myjoblist, job_id, RUNNING);

                            tcsetpgrp(STDIN_FILENO, getpgid(fg_pid));
                            int s;
                            if (waitpid(fg_pid, &s, WUNTRACED) < 0) {
                                perror("waitpid error");
                            }
                            if (WIFSTOPPED(s)) {
                                fprintf(stderr,
                                        "[%d] (%d) suspended by signal %d\n",
                                        job_id, fg_pid, WSTOPSIG(s));
                                update_job_jid(myjoblist, job_id, STOPPED);
                                sig = 1;
                            } else if (WIFSIGNALED(s)) {
                                fprintf(stderr,
                                        "[%d] (%d) terminated by signal %d\n",
                                        job_id, fg_pid, WTERMSIG(s));
                            } else if (WIFCONTINUED(s)) {
                                fprintf(stderr, "[%d] (%d) resumed\n", job,
                                        fg_pid);
                                update_job_jid(myjoblist,
                                               get_job_jid(myjoblist, fg_pid),
                                               RUNNING);
                            }
                            if (sig != 1) {
                                remove_job_pid(myjoblist, fg_pid);
                            }
                            tcsetpgrp(STDIN_FILENO, shellid);
                        }
                    }

                    /* if there is & at the end, then it is a fg process, but we
                     * want to delete & from our argv
                     */

                    fg_bg = 0;
                    int chk = 0;
                    while (chk != SIZE) {
                        if (argv[chk] == NULL) {
                            break;
                        }
                        if (strcmp(argv[chk], "&") == 0) {
                            argv[chk] = NULL;
                            fg_bg = 1;
                        }
                        chk++;
                    }
                    /*if the command is jobs, then we will not put it into
                     * fork*/
                    if (strcmp(tokens[0], "jobs") == 0) {
                        jobs(myjoblist);
                        build_in = 5;
                    }

                    /*
                     fork child process: In this process, we will be swtiching
                     signals, in particular if it is a fg process, we will
                     change our signals to default, and then after we done with
                     it we want to swtich it back and move control to the shell.
                     However, if it is a bg process, we will move it to the
                     shell.
                     */

                    if (build_in == 0) {
                        if ((child = fork()) == 0) {
                            if (child == -1) {
                                perror("fork failed. dead child sad parent");
                            }
			    /*if there is a bg command in the input, this should be inside the child process, we checking if the job is valid and update the job stauts according to the handout.
			     */

                            if (bg_com == 1) {
                                int job_id = (int)(argv[1][1]) - 48;

                                pid_t bg_pid = get_job_pid(myjoblist, job_id);

                                if (bg_pid < 0) {
                                    fprintf(stderr, "job not found\n");
                                }

                                if (kill(-bg_pid, SIGCONT) < 0) {
                                    fprintf(stderr, "job not found\n");
                                }

                                if (job_id > 0) {
                                    update_job_jid(myjoblist, job_id, RUNNING);
                                }
                            }
			    /*We checking those signals and reseting them */
			    
                            if (setpgid(0, 0) < 0) {
                                perror("setpgid fail");
                            }
			    /*if it is a fg process, we send the control of signal to the group id of child. Otherwise return the control back to shell */
                            if (fg_bg == 0) {
                                pid_t pgid;
                                pgid = getpgid(getpid());
                                if (pgid < 0) {
                                    perror("getpgid fail");
                                }

                                tcsetpgrp(STDIN_FILENO, pgid);
                            } else {
                                tcsetpgrp(STDIN_FILENO, shellid);
                            }

                            signal(SIGINT, SIG_DFL);

                            signal(SIGTSTP, SIG_DFL);
                            signal(SIGQUIT, SIG_DFL);

                            int red_in = 0;
                            int count_argu = 0;
                            while (count_argu != 512) {
                                count_argu++;  // counting the number of
                                               // arguments
                                if (argv[count_argu] == NULL) {
                                    break;
                                }
                            }

                            /*
                         The while loop will handle redirection cases, we check
                         those by strcmp whether it is < , > , or >> and we call
                         different modes of open file function properly. Before
                         we open the file, we close the stdin/stdout. If the
                         file is not open properly, we will print out the error.
                             */

                            while (red_in != SIZE) {
                                if (tokens[red_in] == NULL) {
                                    break;
                                }

                                if (strcmp(tokens[red_in], "<") == 0) {
                                    close(0);
                                    fd = open(tokens[red_in + 1], O_RDONLY);
                                    if (fd == -1) {
                                        perror("open error");
                                        exit(1);
                                    }
                                }

                                if (strcmp(tokens[red_in], ">>") == 0) {
                                    close(1);
                                    fd = open(tokens[red_in + 1],
                                              O_CREAT | O_WRONLY | O_APPEND,
                                              0777);
                                    if (fd == -1) {
                                        perror("open error");
                                        exit(1);
                                    }
                                }

                                if (strcmp(tokens[red_in], ">") == 0) {
                                    close(1);
                                    fd = open(tokens[red_in + 1],
                                              O_TRUNC | O_WRONLY | O_CREAT,
                                              0777);
                                    if (fd == -1) {
                                        perror("open error");
                                        exit(1);
                                    }
                                }

                                red_in++;
                            }

                            /*
         if redirection occurs in my code, we must delete the redirection and
the next token in our input lists So we strcmp to check if redirect exists in
input, if so we need to shift every things forwards to replace where redirection
and the next token at.
                             */

                            int clean_argv =
                                0;  // the index we will use to clean up argv
                            while (clean_argv != 500) {
                                if (argv[clean_argv] == NULL) {
                                    break;
                                }
                                if (strcmp(argv[clean_argv], ">") == 0 ||
                                    strcmp(argv[clean_argv], "<") == 0 ||
                                    strcmp(argv[clean_argv], ">>") == 0) {
                                    int tmp = clean_argv;
                                    while (argv[tmp] != NULL) {
                                        argv[tmp] =
                                            argv[tmp +
                                                 2];  // shift the rest of argv
                                                      // forward when
                                                      // redirection be deleted
                                        tmp++;
                                    }

                                    clean_argv = clean_argv - 2;
                                }

                                clean_argv++;
                            }

                            int check_1st_is_output = strcmp(tokens[0], ">");
                            int check_1st_is_input = strcmp(tokens[0], "<");
                            int check_1st_is_append = strcmp(tokens[0], ">>");
                            // edge case when the first input command is
                            // redirection, tokens should be changed as well
                            // since we use tokens as an argument to execv

                            if (check_1st_is_output == 0 ||
                                check_1st_is_input == 0 ||
                                check_1st_is_append == 0) {
                                tokens[0] = tokens[2];
                            }

                            if (strcmp(tokens[0], "jobs") != 0 && bg_com != 1 &&
                                fg_com != 1) {
                                execv(tokens[0], argv);

                                perror("execv");  // error handling for execv
                            }
                            cleanup_job_list(myjoblist);

                            exit(1);
                        }

                        /*
                        parent after fork: We should call waitpid and only wait
                        for fg process, and if it is bg process, we should skip
                        waitpid and add the job into the list.
                        */

                        if (fg_bg == 1) {
                            fprintf(stderr, "[%d] (%d)\n", job, child);
                            add_job(myjoblist, job, child, RUNNING, input);
                            job++;
                        }

                        int status;
                        if (fg_bg == 0) {
                            int sta_w = 0;
                            sta_w = waitpid(child, &status, WUNTRACED);
                            if (sta_w < 0) {
                                perror("waitpid error\n");
                                cleanup_job_list(myjoblist);

                                exit(1);
                            }
                            /* If we terminated by a signal, we should specify
                             * which signal it is by calling WTERMSIG and remove
                             * the job (since it was terminated). And if it is
                             * stopped, we will add it to the job list. If the
                             * SIGCONT was received, we should resume the job
                             * and print the message and update the job status
                             */
                            if (WIFSIGNALED(status)) {
                                fprintf(stderr,
                                        "[%d] (%d) terminated by signal %d\n",
                                        job, child, WTERMSIG(status));
                                remove_job_pid(myjoblist, child);
                            } else if (WIFSTOPPED(status)) {
                                fprintf(stderr,
                                        "[%d] (%d) suspended by signal %d\n",
                                        job, child, WSTOPSIG(status));
                                add_job(myjoblist, job, child, STOPPED, input);
                                job++;
                            } else if (WIFCONTINUED(status)) {
                                fprintf(stderr, "[%d] (%d) resumed\n", job,
                                        child);
                                update_job_jid(myjoblist,
                                               get_job_jid(myjoblist, child),
                                               RUNNING);
                            }
                        }

                        tcsetpgrp(STDIN_FILENO, shellid);
                    }
                }
            }

        } else {
            cleanup_job_list(myjoblist);  // before exit we cleanup the job list
            return 0;
        }
    }
    cleanup_job_list(myjoblist);  // before exit we cleanup the job list

    return 0;
}
/*
check for zombie state, just follow up the lecture slides, we put this function
before the prompt was printed. If the program exit normally or unnormally, we
just print terminated. If there is a signal to terminate the program, we print
terminate and also remove that job from the list...If the job was stopped, we
need to update the job state, changing it to stopped. And if a sigcont was
received, we will just say its resumed.
 */
void check_zombie(job_list_t *myjoblist) {
    int status, pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        if (WIFEXITED(status)) {
            fprintf(stderr, "[%d] (%d) terminated with exit status %d\n",
                    get_job_jid(myjoblist, pid), pid, WEXITSTATUS(status));
            remove_job_jid(myjoblist, get_job_jid(myjoblist, pid));

        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "[%d] (%d) terminated by signal %d\n",
                    get_job_jid(myjoblist, pid), pid, WTERMSIG(status));
            remove_job_pid(myjoblist, pid);
        } else if (WIFSTOPPED(status)) {
            fprintf(stderr, "[%d] (%d) suspended by signal %d\n",
                    get_job_jid(myjoblist, pid), pid, WSTOPSIG(status));
            update_job_jid(myjoblist, get_job_jid(myjoblist, pid), STOPPED);

        } else if (WIFCONTINUED(status)) {
            fprintf(stderr, "[%d] (%d) resumed\n", get_job_jid(myjoblist, pid),
                    pid);

            update_job_jid(myjoblist, get_job_jid(myjoblist, pid), RUNNING);
        }
    }
}

/*
This helper function is to check whether there is a EOF at the very end or the
user hit control d to terminate the program. If that is the case, we should just
cleanup the job list. This helper return 1 when we need to exit, 0 otherwise.
 */
int end_of_file_check(ssize_t checkread, job_list_t *myjoblist) {
    if (checkread == 0 || checkread == EOF) {
        cleanup_job_list(myjoblist);

        return 1;
    }
    return 0;
}

/*
This helper function takes token string as input, as will check if there are
build in commands. The return value is 0, if there is no build in command, and
return 5 if there is a build in command.
 */
int check_build_in(char *tokens[512]) {
    if (strcmp(tokens[0], "cd") == 0) {
        if (tokens[1] == NULL) {
            fprintf(stderr, "cd: syntax error\n");

        } else {
            if (chdir(tokens[1]) == -1) {
                perror("error in cd the file");
            }  // to cd the directory
        }
        return 5;
    } else if (strcmp(tokens[0], "rm") == 0) {
        if (unlink(tokens[1]) == -1) {
            perror("error in remove a file");
        }  // to remove a file
        return 5;
    } else if (strcmp(tokens[0], "ln") == 0) {
        if (link(tokens[1], tokens[2]) == -1) {
            perror("error in link files");
        }  // to hard link files
        return 5;
    } else {
        return 0;
    }

    return 0;
}
