#ifdef BUILTIN_STRINGS
#include <string.h>
#else
#include "./strings.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "./sec.h"
#include "./strings_unit_tests.h"

#define USAGE                                                            \
    "./run_tests <all | "                                                \
    "names of tests to run>"                                             \
    "\n   Ex. \"./run_tests all\" runs all tests"                        \
    "\n   Ex. \"./run_tests strlen strchr\" runs the strlen and strchr " \
    "\n   Ex. \"./run_tests strtok\" runs the strtok test"

#define SUPER_LONG_STRING                                                    \
    "./run_tests"                                                            \
    "<all | names of tests to run>"                                          \
    "\n   Ex. \"./run_tests 10000 all\" runs all tests 10000 times each"     \
    "\n   Ex. \"./run_tests 100 strlen strchr\" runs the strlen and strchr " \
    "tests 100 times each"                                                   \
    "\n   Ex. \"./run_tests strtok\" runs the strtok test 1000000 times"     \
    "./run_tests <all | "                                                    \
    "names of tests to run>"                                                 \
    "\n   Ex. \"./run_tests 10000 all\" runs all tests 10000 times each"     \
    "\n   Ex. \"./run_tests 100 strlen strchr\" runs the strlen and strchr " \
    "tests 100 times each"                                                   \
    "\n   Ex. \"./run_tests strtok\" runs the strtok test 1000000 times"     \
    "./run_tests <all | "                                                    \
    "names of tests to run>"                                                 \
    "\n   Ex. \"./run_tests 10000 all\" runs all tests 10000 times each"     \
    "\n   Ex. \"./run_tests 100 strlen strchr\" runs the strlen and strchr " \
    "tests 100 times each"                                                   \
    "\n   Ex. \"./run_tests strtok\" runs the strtok test 1000000 times"

int num_repeat;

extern int should_print;
extern int should_count;

int should_time = 0;

void time_test(void (*)(), int, const char *);

// test functions
// size_t strlen(const char *s);
void test_strlen()
{
    char s[350];
    snprintf(s, sizeof(s) / sizeof(char), "strlen(\"%s\")", USAGE);

    assert_equal_int(0, strlen(""), "strlen(\"\")");
    assert_equal_int(4, strlen("asdf"), "strlen(\"asdf\")");
    assert_equal_int(5, strlen("asdf "), "strlen(\"asdf \")");
    assert_equal_int(2, strlen("\n\n"), "strlen(\"\n\n\")");
    assert_equal_int(193, strlen(USAGE), s);
}

// char *strchr(const char *s, int c);
void test_strchr()
{
    char *str1 = "12345";
    assert_equal_str(str1, strchr(str1, '1'), "strchr(\"12345\", '1')");
    assert_equal_str((char *)&str1[1], strchr(str1, '2'),
                     "strchr(\"12345\", '2')");
    assert_equal_str(NULL, strchr(str1, '6'), "strchr(\"12345\", '6')");

    char *str2 = "brown university";
    assert_equal_str((char *)&str2[6], strchr(str2, 'u'),
                     "strchr(\"brown university\", 'u')");

    char s[350];
    snprintf(s, sizeof(s) / sizeof(char), "strchr(\"%s\", '.')", USAGE);
    assert_equal_str(USAGE, strchr(USAGE, '.'), s);
}

// char *strcpy(char *dest, const char *src);
void test_strcpy() {}

// char *strncpy(char *dest, const char *src, size_t n);
void test_strncpy()
{
    char msg_buf[1024];
    char abbreviation[5];
    char *src = "professor";
    strncpy(abbreviation, src, 3);

    for (int i = 0; i <= 2; i++)
    {
        snprintf(msg_buf, sizeof(msg_buf) / sizeof(char),
                 "strncpy(..., \"%s\", 3) char #%d", src, i);
        assert_equal_char(src[i], abbreviation[i], msg_buf);
    }

    char buffer[10];
    strncpy(buffer, "brown", 10);
    assert_equal_str("brown", buffer, "strncpy(..., \"brown\", 10);");
    for (int i = 5; i < 10; ++i)
        assert_equal_char('\0', buffer[i],
                          "strncpy(..., \"brown\", 10) null bytes at end");
}

// size_t strspn(const char *s, const char *accept);
void test_strspn()
{
    assert_equal_int(2, strspn("professor", "rpf"),
                     "strspn(\"professor\", \"rpf\")");
    assert_equal_int(0, strspn("bin/each/apple", "/"),
                   "strspn(\"bin/each/apple\", \"/\")");

    assert_equal_int(0, strspn("professor", ""), "strspn(\"professor\", \"\")");

    char long_str_err[1000];
    snprintf(long_str_err, sizeof(long_str_err) / sizeof(char),
             "strspn(\"%s\", \"r/.\")", SUPER_LONG_STRING);
    assert_equal_int(3, strspn(SUPER_LONG_STRING, "r/."), long_str_err);
}

// size_t strcspn(const char *s, const char *reject);
void test_strcspn()
{
    assert_equal_int(0, strcspn("professor", "rpf"),
                     "strcspn(\"professor\", \"rpf\")");
    assert_equal_int(9, strcspn("professor", ""),
                     "strcspn(\"professor\", \"\")");
    assert_equal_int(4, strcspn("professor", "es"),
                     "strcspn(\"professor\", \"es\")");

    char long_str_err[1000];
    snprintf(long_str_err, sizeof(long_str_err) / sizeof(char),
             "strcspn(\"%s\", \"r/.\")", SUPER_LONG_STRING);
    assert_equal_int(0, strcspn(SUPER_LONG_STRING, "r/."), long_str_err);
}

// int strcmp(const char *s1, const char *s2);
void test_strcmp()
{
    assert_same_sign(-2, strcmp("professor", "rpf"),
                     "strcmp(\"professor\", \"rpf\")");
    assert_same_sign(17, strcmp("test", "case"), "strcmp(\"test\", \"case\")");
    assert_same_sign(0, strcmp("abc", "abc"), "strcmp(\"abc\", \"abc\")");
    assert_same_sign(101, strcmp("one", "on"), "strcmp(\"one\", \"on\")");
    assert_same_sign(114, strcmp("rpf", ""), "strcmp(\"rpf\", \"\")");

    char long_str_err[1000];
    char slightly_different_long_str[1000];
    snprintf(slightly_different_long_str,
             sizeof(slightly_different_long_str) / sizeof(char), "x%s", USAGE);
    snprintf(long_str_err, sizeof(long_str_err) / sizeof(char),
             "strcmp(\"%s\", \"%s\")", USAGE, slightly_different_long_str);
    assert_same_sign(-74, strcmp(USAGE, slightly_different_long_str),
                     long_str_err);
}

// int strncmp(const char *s1, const char *s2, size_t n);
void test_strncmp()
{
    assert_same_sign(-2, strncmp("professor", "rpf", 3),
                     "strncmp(\"professor\", \"rpf\", 3)");
    assert_same_sign(17, strncmp("test", "case", 1),
                     "strncmp(\"test\", \"case\", 1)");
    assert_same_sign(0, strncmp("abc", "abc", 5),
                     "strncmp(\"abc\", \"abc\", 5)");
    assert_same_sign(0, strncmp("asdf", "asde", 3),
                     "strncmp(\"asdf\", \"asde\", 3)");
    assert_same_sign(114, strncmp("rpf", "", 3), "strncmp(\"rpf\", \"\", 3)");
    assert_same_sign(0, strncmp("rpf", "", 0), "strncmp(\"rpf\", \"\", 0)");

    char long_str_err[750];
    char slightly_different_long_str[350];
    snprintf(slightly_different_long_str,
             sizeof(slightly_different_long_str) / sizeof(char), "%sx", USAGE);
    snprintf(long_str_err, sizeof(long_str_err) / sizeof(char),
             "strncmp(\"%s\", \"%s\", 5)", USAGE, slightly_different_long_str);
    assert_same_sign(0, strncmp(USAGE, slightly_different_long_str, 5),
                     long_str_err);
}

// char *strstr(const char *haystack, const char *needle);
void test_strstr()
{
    char *eg = "eg";
    char *dg = "dg";
    char *f = "f";
    assert_equal_str("defg", strstr("abcdefg", "de"),
                         "strstr(\"abcdefg\", \"de\")");
    assert_equal_str("babab", strstr("abbabab", "bab"),
                   "strstr(\"abbabab\", \"bab\")");
    assert_equal_str("eg", strstr("abcdeg", eg), "strstr(\"abcdeg\", \"eg\")");
    assert_equal_str(NULL, strstr("abcdeg", dg), "strstr(\"abcdeg\", \"dg\")");
    assert_equal_str(NULL, strstr("abcdeg", f), "strstr(\"abcdeg\", \"f\")");
    assert_equal_str("eg", strstr("eg", eg), "strstr(\"eg\", \"eg\")");
}

// char *strtok(char *str, const char *delim);
void test_strtok()
{
    char str[80] = "1,2,,3,4";
    const char s[2] = ",";
    assert_equal_str("1", strtok(str, s),
                     "strtok(\"1,2,,3,4\", \",\") first time");
    assert_equal_str("2", strtok(NULL, s),
                     "strtok(\"1,2,,3,4\", \",\") second time");
    assert_equal_str("3", strtok(NULL, s),
                     "strtok(\"1,2,,3,4\", \",\") third time");
    assert_equal_str("4", strtok(NULL, s),
                     "strtok(\"1,2,,3,4\", \",\") fourth time");
    assert_equal_str(NULL, strtok(NULL, s),
                     "strtok(\"1,2,,3,4\", \",\") fifth time");

    char str2[80] = ";;;33;;6;;77;";
    const char d[3] = ";;";
    assert_equal_str("33", strtok(str2, d),
                     "strtok(\";;;33;;6;;77;\", \";;\") first time");
    assert_equal_str("6", strtok(NULL, d),
                     "strtok(\";;;33;;6;;77;\", \";;\") second time");
    assert_equal_str("77", strtok(NULL, d),
                     "strtok(\";;;33;;6;;77;\", \";;\") third time");
    assert_equal_str(NULL, strtok(NULL, d),
                     "strtok(\";;;33;;6;;77;\", \";;\") fourth time");
}

extern int total_tests, num_correct, num_incorrect;

int run_test_in_separate_process(void (*func)(), int num_tests,
                                 const char *message)
{
    int pid, status;
    if (!(pid = fork()))
    {
        func();
        if (strcmp_test(message, "all"))
        {
            printf("%s: ", message);
            print_test_summary();
        }

        // Return 0 if no tests failed
        exit(!!num_incorrect);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
    {
        // Did we return 0
        return WEXITSTATUS(status) == 0;
    }
    printf(
        "%s: Passed 0 tests, Failed %d tests | 0.00% (process exited with "
        "code: %d)\n",
        message, num_tests, status);

    return 0;
}

void test_all(int *passed, int *num_tests)
{
    int functions_passed = 0;
    functions_passed += run_test_in_separate_process(&test_strlen, 5, "strlen");
    functions_passed += run_test_in_separate_process(&test_strspn, 3, "strspn");
    functions_passed += run_test_in_separate_process(&test_strcspn, 4, "strcspn");
    functions_passed += run_test_in_separate_process(&test_strncmp, 7, "strncmp");
    functions_passed += run_test_in_separate_process(&test_strstr, 6, "strstr");

    // UPDATE THIS!!!
    *passed = functions_passed;
    *num_tests = 5;
}

void foreach_test(int num_tests, char const *test_names[],
                  int (*wrapper)(void (*)(), int, const char *))
{
    int functions_passed = 0;
    int dummy;

    if (!strcmp_test(test_names[1], "all"))
    {
        test_all(&functions_passed, &num_tests);
        printf("%d/%d functions correctly implemented\n", functions_passed, num_tests);
        return;
    }

    for (int i = 0; i < num_tests; ++i)
    {
        if (i == 0)
            continue; // file name

        const char *test_name = test_names[i];
        if (!strcmp_test(test_name, "strlen"))
            functions_passed += wrapper(&test_strlen, 5, "strlen");
        else if (!strcmp_test(test_name, "strchr"))
            functions_passed += wrapper(&test_strchr, 0, "strchr");
        else if (!strcmp_test(test_name, "strspn"))
            functions_passed += wrapper(&test_strspn, 3, "strspn");
        else if (!strcmp_test(test_name, "strcspn"))
            functions_passed += wrapper(&test_strcspn, 4, "strcspn");
        else if (!strcmp_test(test_name, "strcmp"))
            functions_passed += wrapper(&test_strcmp, 0, "strcmp");
        else if (!strcmp_test(test_name, "strncmp"))
            functions_passed += wrapper(&test_strncmp, 5, "strncmp");
        else if (!strcmp_test(test_name, "strstr"))
            functions_passed += wrapper(&test_strstr, 7, "strstr");
        else if (sscanf(test_name, "%d", &dummy) != 1)
            printf("Unknown test: %s\n", test_name);
    }

    printf("%d/%d functions correctly implemented\n", functions_passed, num_tests - 1);
}

int main(int argc, char const *argv[])
{
    /* code */
    printf("Testing for correctness...\n");

    if (argc < 2)
    {
        printf("USAGE: %s\n", USAGE);
        return 1;
    }
    install_seccomp();

    foreach_test(argc, argv, &run_test_in_separate_process);

    return 0;
}
