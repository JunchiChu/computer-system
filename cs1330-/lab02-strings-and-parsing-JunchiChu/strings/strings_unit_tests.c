#include <math.h>
#include <stdio.h>

int should_print = 1;
int should_count = 1;

int total_tests = 0;
int num_correct = 0;
int num_incorrect = 0;

int strcmp_test(const char *s1, const char *s2);
size_t strlen_test(const char *s);

void assert_equal_int(int expected, int actual, const char *message)
{
    if (!should_count)
        return;

    ++total_tests;
    if (expected == actual)
    {
        ++num_correct;
    }
    else
    {
        ++num_incorrect;
        if (should_print)
            printf("FAILED Test %d: %s]: Expected %d, actual %d\n", total_tests,
                   message, expected, actual);
    }
}

void assert_same_sign(int expected, int actual, const char *message)
{
    if (!should_count)
        return;

    ++total_tests;
    // if both are 0, both positive, or both negative
    if ((actual == 0 && expected == 0) || ((expected > 0) && (actual > 0)) ||
        ((expected < 0) && (actual < 0)))
    {
        ++num_correct;
    }
    else
    {
        ++num_incorrect;
        if (should_print)
            printf(
                "[FAILED Test %d: %s]: Expected %d, actual %d (should be the "
                "same sign)\n",
                total_tests, message, expected, actual);
    }
}

void assert_equal_char(char expected, char actual, const char *message)
{
    if (!should_count)
        return;

    ++total_tests;
    if (expected == actual)
    {
        ++num_correct;
    }
    else
    {
        ++num_incorrect;
        if (should_print)
            printf("[FAILED Test %d: %s]: Expected %c, actual %c\n",
                   total_tests, message, expected, actual);
    }
}

void assert_equal_str(char *expected, char *actual, const char *message)
{
    if (!should_count)
        return;

    ++total_tests;

    if (expected == NULL || actual == NULL)
    {
        if (expected == NULL && actual == NULL)
        {
            ++num_correct;
        }
        else
        {
            ++num_incorrect;
            if (should_print)
                printf("[FAILED Test %d: %s]: Expected %s, actual %s\n",
                       total_tests, message, expected, actual);
        }
        return;
    }

    if (!strcmp_test(expected, actual))
    {
        ++num_correct;
    }
    else
    {
        ++num_incorrect;
        if (should_print)
            printf("[FAILED Test %d: %s]: Expected %s, actual %s\n",
                   total_tests, message, expected, actual);
    }
}

void print_test_summary()
{
    printf(
        "Passed %d tests, Failed %d tests | %.2f%%\n", num_correct, num_incorrect,
        100 * ceil((float)num_correct) / (total_tests == 0 ? 1 : total_tests));
}

// TA code for strcmp
int strcmp_test(const char *s1, const char *s2)
{
    register size_t i;

    // Determine the length of the shortest string.

    // For each character in the shortest string, see if the strings diverge.
    for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++)
        if (s1[i] != s2[i])
            return s1[i] - s2[i];

    // i should be equal to min(strlen(s1), strlen(s2)) if code gets here
    // one string must have a null byte. if both have null byte, return 0
    // otherwise, return s1-s2 at end
    return s1[i] - s2[i];
}