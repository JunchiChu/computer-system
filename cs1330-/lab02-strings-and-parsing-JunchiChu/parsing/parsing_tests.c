#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"

// ANSI UI Stuff: 
#define DEFAULT "\033[0m"

#define BOLD(str) "\033[1m" str DEFAULT

#define GREEN(str) "\033[32m" str DEFAULT

#define RED(str) "\033[31m" str "\033[0;0m"

#define BOLD_GREEN(str) "\033[1m\033[32m" str DEFAULT

#define BOLD_RED(str) "\033[1m\033[31m" str DEFAULT

#define PRINT_HR() puts("================================================================================");

#define PRINT_HD() puts("--------------------------------------------------------------------------------");



typedef struct token_test {
   char* test_name;
   char* input;
   char* expected_tokens[128];
   char* expected_execv[128];

} token_test_t;

token_test_t tests[128];
int __test_count = 0;

void add_test(token_test_t t) {
    tests[__test_count++] = t;
}

void print_tokens(char* prefix, char** tokens, int max) {
    
    char** cur = tokens;
    int i = 0;

    do {
        printf("%s[%d]: %s\n", prefix, i, *cur);
        cur++;
        i++;
    } while(*cur && i < max);

    if (i == max) {
        printf("Max tokens exceeded! Did you NULL terminate?\n");
    } else {
        printf("%s[%d]: (null)\n", prefix, i);
    }
}

int check_tokens(char** a, char** b)
{
    char** cura = a;
    char** curb = b;

    while(1)
    {   
        if (!*cura && !*curb) {
            // Both NULL: True
            return 1;
        } else if (!*cura || !*curb) {
            // One NULL but not the other: False
            return 0;
        } else if (strcmp(*cura, *curb) != 0) {
            // String mismatch False
            return 0;
        }

        cura++;
        curb++;
    }
}

int run_test(token_test_t* test, int idx)
{
    printf(BOLD("TEST %d: %s\n"), idx, test->test_name);
    PRINT_HR()
    printf(BOLD("INPUT: %s\n"), test->input);


    char buffer[1024];
    memset(buffer, 0, 1024);
    char *tokens[512 * sizeof(char *)];
    memset(tokens, 0, 512 * sizeof(char *));
    char *execv[512 * sizeof(char *)];
    memset(execv, 0, 512 * sizeof(char *));
    strncpy(buffer, test->input, 1024);
    parse(buffer, tokens, execv);


    printf(BOLD("\nExecv:\n"));
    PRINT_HD();

    print_tokens("expected", test->expected_execv, 10);
    print_tokens("received", execv, 10);

    
    printf(BOLD("\nTokens:\n"));
    PRINT_HD();

    print_tokens("expected", test->expected_tokens, 10);
    print_tokens("received", tokens, 10);

    
    int result = check_tokens(test->expected_execv, execv) && check_tokens(test->expected_tokens, tokens);

    if (result) { 
        printf(BOLD("\nResult: ") BOLD_GREEN("PASSED %lc\n"), 0x2714);
    } else {

        printf(BOLD("\nResult: ") BOLD_RED("FAILED %lc\n"), 0x2717);
    }
    printf(DEFAULT "\n\n");
    return result;
}

void run_tests() 
{   
    int passed = 0;
    for(int i = 0; i < __test_count; i++)
    {
        int result = run_test(&tests[i], i + 1);
        if (result) {
            passed++;
        }
    }
    printf("%d/%d tests passed\n", passed, __test_count);
}
#include <locale.h>

int main(int argc, char* argv[])
{   
    setlocale(LC_ALL, "");
    /* cd test */
    add_test((token_test_t) {
        .test_name = "simple `cd` behavior",
        .input = "cd directory",
        .expected_tokens = {
            "cd",
            "directory",
            NULL
        },
        .expected_execv = {
            "cd",
            "directory",
            NULL
        }
    });
    
    /* */
    add_test((token_test_t) {
        .test_name = "execv binary name handling",
        .input = "/bin/cd directory",
        .expected_tokens = {
            "/bin/cd",
            "directory",
            NULL
        },
        .expected_execv = {
            "cd",
            "directory",
            NULL
        }
    });


    /* */
    add_test((token_test_t) {
        .test_name = "empty string",
        .input = "",
        .expected_tokens = {
            NULL
        },
        .expected_execv = {
            NULL
        }
    });

    /* */
    add_test((token_test_t) {
        .test_name = "whitespace",
        .input = " ",
        .expected_tokens = {
            NULL
        },
        .expected_execv = {
            NULL
        }
    });

    /* */
    add_test((token_test_t) {
        .test_name = "tab",
        .input = "\techo\t",
        .expected_tokens = {
            "echo",
            NULL
        },
        .expected_execv = {
            "echo",
            NULL
        }
    });

    /* */
    add_test((token_test_t) {
        .test_name = "tab and whitespace",
        .input = "\techo ",
        .expected_tokens = {
            "echo",
            NULL
        },
        .expected_execv = {
            "echo",
            NULL
        }
    });


    /* */
    add_test((token_test_t) {
        .test_name = "tab and whitespace",
        .input = "\techo ",
        .expected_tokens = {
            "echo",
            NULL
        },
        .expected_execv = {
            "echo",
            NULL
        }
    });


    /* */
    add_test((token_test_t) {
        .test_name = "multiple arguments",
        .input = "echo trader toms snacks",
        .expected_tokens = {
            "echo",
            "trader",
            "toms",
            "snacks",
            NULL
        },
        .expected_execv = {
            "echo",
            "trader",
            "toms",
            "snacks",
            NULL
        },
    });


    /* */
    add_test((token_test_t) {
        .test_name = "execv",
        .input = "/bin/ls",
        .expected_tokens = {
            "/bin/ls",
            NULL
        },
        .expected_execv = {
            "ls",
            NULL
        },
    });

    /* */
    add_test((token_test_t) {
        .test_name = "multiple arguments",
        .input = "/bin/echo 'Hello World!'",
        .expected_tokens = {
            "/bin/echo",
            "'Hello",
            "World!'",
            NULL
        },
        .expected_execv = {
            "echo",
            "'Hello",
            "World!'",
            NULL
        },
    });

    /* */
    add_test((token_test_t) {
        .test_name = "invalid command",
        .input = "/bin/ls/",
        .expected_tokens = {
            "/bin/ls/",
            NULL
        },
        .expected_execv = {
            "",
            NULL
        },
    });

    /* */
    add_test((token_test_t) {
        .test_name = "long command",
        .input = "/course/cs0330/test",
        .expected_tokens = {
            "/course/cs0330/test",
            NULL
        },
        .expected_execv = {
            "test",
            NULL
        },
    });


    run_tests();

    return 0;
}