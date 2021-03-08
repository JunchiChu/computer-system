int should_print, should_count;

#define RESUME_COUNTING should_count = 1;
#define STOP_COUNTING should_count = 0;

#define RESUME_PRINTING should_print = 1;
#define STOP_PRINTING should_print = 0;

void assert_equal_int(int expected, int actual, const char *message);
void assert_same_sign(int expected, int actual, const char *message);
void assert_equal_char(char expected, char actual, const char *message);
void assert_equal_str(char *expected, char *actual, const char *message);
void print_test_summary();
int strcmp_test(const char *s1, const char *s2);
