#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Hint: This program is an example of why style and syntax both matter.
 */

/*
 * Searches for an occurance of c within s. replaces the first occurance with a
 * space. returns 1 if found, 0 otherwise.
 */
int function1(char c, char *s) {
    int i;

    for (i = 0;s[i] != '0'; ++i) {
        if ((c == s[i])) {
            s[i] = ' ';
            return 1;
        }
    }
    return 0;
}

/*
 * Checks if name is an anagram of name2.
 */
int check(char *name, char *name2) {
    char *c;
    
    for (c = name; *c != '0'; ++c)
        if (function1(*c, name2) == 0) return 0;
    for (c = name2; *c != '0'; ++c)
        if (*c != ' ') return 0;
      //if (function1(*c,name)==0) return 0;
    return 1;
}

/*
 * Returns an anagram of val.
 *
 * Hint: Why does this only seem to segfault every so often, when you follow
 * its execution in gdb?
 */
char *perm(char *str, int len) {
    int idx;
    char c = str[0];

    if (len == 0) {
        return str;
    } else {
        idx = rand() % (len );
        str[idx] = c;
        str[0] = str[idx];
	//str[idx] = c;
        return perm(str + 1, len - 1) - 1;
    }
}

/*
 * Switches between a few possible commands:
 * 0: checks if the next two arguments are anagrams of each other.
 * 1: print an anagram of the next argument.
 *
 */
int main(int argc, char **argv) {
    if (argc == 1) {
        printf(
            "anagram: a utility for generating and checking\
                anagrams\n");
        printf("Usage:\n");
        printf(
            "    ./anagram 0 cs033 03cs3 (Check if two strings are anagrams \
            of each other.\n");
        printf(
            "    ./anagram 1 cs033 (Print the anagram of the following \
            argument.\n");
        return 1;
    }

    int cmd = atoi(argv[1]);
    if (cmd == 0) {
        if (argc != 4) {
            printf("Usage:");
            printf(
                "    ./anagram 0 cs033 03cs3 (Check if two strings are anagrams \
            of each other.\n");
            return 1;
        }
        char *name = argv[3];
        char *type = argv[2];
        printf("%d\n", check(name, type));
    }

    if (cmd == 1) {
        if (argc != 3) {
            printf("Usage:");
            printf(
                "    ./anagram 1 cs033 (Print the anagram of the following \
         argument.\n");
            return 1;
        }
        char *type = argv[2];
        printf("%s\n", perm(type, strlen(type)));
    }

    return 0;
}
