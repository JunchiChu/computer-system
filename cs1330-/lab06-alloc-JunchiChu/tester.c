#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "linkedlist.h"
#include "string_fns.h"

#define BUF_SIZE 1024

// Forward declarations
void print_help();
char *skip_ws(char *str);
char *skip_nonws(char *str);
void next_word(char **currp, char **nextp);
int add_file(LinkedList *linkedlist, char *file);
int remove_file(LinkedList *linkedlist, char *curr);

int main(int argc, char **argv) {
    char buf[BUF_SIZE];

    LinkedList *linkedlist =
        linkedlist_new(&string_equals, &string_copy, &string_delete);
    if (!linkedlist) {
        fprintf(stderr, "linkedlist_new() returned NULL\n");
        return 1;
    }

    linkedlist_print(linkedlist, stdout, &string_print);
    printf("\n\n");

    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(buf, BUF_SIZE, stdin) == NULL) break;

        // preprocess comment out of buffer
        char *comment = strstr(buf, "//");
        if (comment != NULL) {
            *comment = 0;
        }

        char *curr;
        char *next = buf;
        next_word(&curr, &next);

        if (!*curr) {
            // Blank line -- do nothing
            continue;
        } else if (!strcasecmp(curr, "a")) {
            next_word(&curr, &next);

            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                linkedlist_append(linkedlist, (void *)curr);
                printf("Element appended\n");
                linkedlist_print(linkedlist, stdout, &string_print);
                printf("\n");
            }
        } else if (!strcasecmp(curr, "i")) {
            next_word(&curr, &next);
            if (!*(skip_ws(next))) {
                printf("Too few arguments\n");
            } else {
                char *str = curr;
                next_word(&curr, &next);
                if (*(skip_ws(next))) {
                    printf("Too many arguments\n");
                } else {
                    char *index = curr;
                    if (linkedlist_insert(linkedlist, (void *)str, atoi(index)))
                        printf("Element inserted\n");
                    else
                        printf("Index is invalid\n");
                }
                linkedlist_print(linkedlist, stdout, &string_print);
                printf("\n");
            }
        } else if (!strcasecmp(curr, "c")) {
            next_word(&curr, &next);
            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                printf(linkedlist_contains(linkedlist, (void *)curr)
                           ? "true\n"
                           : "false\n");
            }
        } else if (!strcasecmp(curr, "r")) {
            next_word(&curr, &next);
            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                if (linkedlist_remove(linkedlist, (void *)curr))
                    printf("Element removed\n");
                else
                    printf("Element not found\n");
                linkedlist_print(linkedlist, stdout, &string_print);
                printf("\n");
            }
        } else if (!strcasecmp(curr, "x")) {
            next_word(&curr, &next);
            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                linkedlist_delete(linkedlist);
                linkedlist = linkedlist_new(&string_equals, &string_copy,
                                            &string_delete);
                if (!linkedlist) {
                    fprintf(stderr, "linkedlist_new() returned NULL\n");
                    return 1;
                }
                linkedlist_print(linkedlist, stdout, &string_print);
                printf("\n");
            }
        } else if (!strcasecmp(curr, "s")) {
            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                printf("%d\n", linkedlist_size(linkedlist));
            }
        } else if (!strcasecmp(curr, "p")) {
            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                linkedlist_print(linkedlist, stdout, &string_print);
                printf("\n");
            }
        } else if (!strcasecmp(curr, "fa")) {
            next_word(&curr, &next);

            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                if (add_file(linkedlist, curr)) {
                    printf("error loading file.\n");
                } else {
                    linkedlist_print(linkedlist, stdout, &string_print);
                    printf("\n");
                }
            }
        } else if (!strcasecmp(curr, "fr")) {
            next_word(&curr, &next);

            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                if (remove_file(linkedlist, curr)) {
                    printf("error loading file.\n");
                } else {
                    linkedlist_print(linkedlist, stdout, &string_print);
                    printf("\n");
                }
            }
        } else if (!strcasecmp(curr, "quit")) {
            if (*(skip_ws(next))) {
                printf("Too many arguments\n");
            } else {
                break;
            }
        } else if (!strcasecmp(curr, "h") || !strcasecmp(curr, "help")) {
            print_help();
        } else {
            printf("Invalid command\n");
        }

        printf("\n");
    }

    if (ferror(stdin)) perror("fgets() failed");

    linkedlist_delete(linkedlist);

    return 0;
}

/* Add contents of file to linkedlist */
int add_file(LinkedList *linkedlist, char *curr) {
    FILE *f = fopen(curr, "r");
    if (!f) {
        fprintf(stderr, "error opening file %s: %s\n", curr, strerror(errno));
        return 1;
    }

    char buf[BUF_SIZE];
    while (fgets(buf, BUF_SIZE, f)) {
        char *curr;
        char *next = buf;

        next_word(&curr, &next);
        while (*curr) {
            linkedlist_append(linkedlist, (void *)curr);
            printf("added \"%s\"\n", curr);
            next_word(&curr, &next);
        }
    }

    fclose(f);
    return 0;
}

/* Add contents of file to linkedlist */
int remove_file(LinkedList *linkedlist, char *curr) {
    FILE *f = fopen(curr, "r");
    if (!f) {
        fprintf(stderr, "error opening file %s: %s\n", curr, strerror(errno));
        return 1;
    }

    char buf[1024];
    while (fgets(buf, 1024, f)) {
        char *curr;
        char *next = buf;

        next_word(&curr, &next);
        while (*curr) {
            if (linkedlist_remove(linkedlist, (void *)curr))
                printf("removed \"%s\"\n", curr);
            next_word(&curr, &next);
        }
    }

    fclose(f);
    return 0;
}

/* Print formatted help information about the given command */
void print_command(const char *name, const char *explanation) {
    printf("  %-9s  %s\n", name, explanation);
}

/* Print help information */
void print_help() {
    printf("The following commands are avaiable:\n");

    print_command("a <str>", "Add <str> to the list");
    print_command("i <str> <index>", "Add <str> to the list before <index>");
    print_command("c <str>", "Test if <str> is in the list");
    print_command("r <str>", "Remove <str> from the list");
    print_command("x", "Delete and re-instantiate the list");

    print_command("s", "Print the list size");
    print_command("p", "Print a representation of the list");

    print_command("fa <file>", "Add each word of <file> to the list");
    print_command("fr <file>", "Remove each word of <file> from the list");

    print_command("help", "Print help information");
    print_command("quit", "Delete the list and exit the program");
}

/* Advance pointer until first non-whitespace character */
char *skip_ws(char *str) {
    while (isspace(*str)) str++;
    return str;
}

/* Advance pointer until first whitespace or null character */
char *skip_nonws(char *str) {
    while (*str && !(isspace(*str))) str++;
    return str;
}

/* Advance to the next word and null-terminate */
void next_word(char **currp, char **nextp) {
    *currp = skip_ws(*nextp);
    *nextp = skip_nonws(*currp);
    if (**nextp) {
        **nextp = 0;
        (*nextp)++;
    }
}
