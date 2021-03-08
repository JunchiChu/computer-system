#include <fcntl.h>  // for open
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>  // for close
#include <unistd.h>
#define MAXSIZE 1024
#define SIZE 512
int main() {
    int i = 10;
    i = 10;

    while (i != 0) {
        pid_t child;
        char input[MAXSIZE];
        ssize_t checkread;

        char *tokenn;
        char *arg;
        char *art;
        int fd = 0;
#ifdef PROMPT
        if (printf("33sh> ") < 0) {
            perror("prompt error");
        }
        if (fflush(stdout) < 0) {
            perror("fflush error");
        }
#endif

        checkread = read(STDIN_FILENO, input, MAXSIZE);

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

                    if (strstr(tokens[0], "exit") != NULL) {
                        exit(0);  // if the user want to exit, we should exit
                                  // properly.
                    }

                    /*
build in is a flag to indicate whether it is the case we should run our command
in build in way or we should call execv to run the command.
                     */

                    int build_in = 0;
                    if (build_in == 0) {
                        if (strcmp(tokens[0], "cd") == 0) {
                            if (tokens[1] == NULL) {
                                fprintf(stderr, "cd: syntax error\n");

                            } else {
                                if (chdir(tokens[1]) == -1) {
                                    perror("error in cd the file");
                                }  // to cd the directory
                            }
                            build_in = 5;
                        } else if (strcmp(tokens[0], "rm") == 0) {
                            build_in = 5;
                            if (unlink(tokens[1]) == -1) {
                                perror("error in remove a file");
                            }  // to remove a file
                        } else if (strcmp(tokens[0], "ln") == 0) {
                            build_in = 5;
                            if (link(tokens[1], tokens[2]) == -1) {
                                perror("error in link files");
                            }  // to hard link files
                        } else {
                            build_in = 0;
                        }
                    }

                    /*
           if build in is 0 that indicates we should use fork to create a child
process Then we need to run execv to process our command!
                     */
                    if (build_in == 0) {
                        if ((child = fork()) == 0) {
                            if (child == -1) {
                                perror("fork failed. dead child sad parent");
                            }
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

                            execv(tokens[0], argv);

                            perror("execv");  // error handling for execv
                            exit(1);
                        }

                        int sta_wait;
                        sta_wait = wait(NULL);
                        if (sta_wait < 0) {
                            perror("wait");
                        }
                    }
                }
            }

        } else {
            return 0;
        }
    }
    return 0;
}
