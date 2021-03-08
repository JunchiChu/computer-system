#include <stdio.h>
#include "./lower.h"
#include "./upper.h"

#define INPUT_BUF_SIZE 256

char *whisper(char *sentence) {
    char *sentence_beginning = sentence;
    char current_letter = *sentence;
    while (current_letter != '\0') {
        *sentence = to_lowercase(current_letter);
        sentence++;
        current_letter = *sentence;
    }

    return sentence_beginning;
}

char *shout(char *sentence) {
    char *sentence_beginning = sentence;
    char current_letter = *sentence;
    int uppercase_next_letter = 1;

    while (current_letter != '\0') {
        if (uppercase_next_letter) {
            *sentence = to_uppercase(current_letter);
        }
#ifdef EXTRA_LOUD
        uppercase_next_letter = 1;
#else
        uppercase_next_letter = (current_letter == ' ');
#endif
        sentence++;
        current_letter = *sentence;
    }

    return sentence_beginning;
}

int main() {
    char input_buf[INPUT_BUF_SIZE];
    char * res = fgets(input_buf, INPUT_BUF_SIZE, stdin);
    if (res == NULL) return 1;
    int should_shout = 1;

    while (!feof(stdin)) {
        if (should_shout) {
            printf("Shout: ");
            printf("%s", shout(input_buf));
        } else {
            printf("Whisper: ");
            printf("%s", whisper(input_buf));
        }

        printf("\n");

        res = fgets(input_buf, INPUT_BUF_SIZE, stdin);
        if (res == NULL) return 1;

        should_shout = !should_shout;
    }
}
