#include <stdio.h>
#include <stdlib.h>

/*
 * Hint: vector_pointers is an array of pointers to the elements of the original
 * vector. What should be happening to it? If something goes wrong with it, you
 * should show that directly.
 */

// What's the deal with this function?
void edit_thing(int **x, int y) { *(x + y) = 0; }

/*
 * Return the value at a location from a vector of pointers after the index
 * denoted by i.
 */
int get_next_element(int *vector_pointers[], int i) {
    return *(vector_pointers[i + 1]);
}

/*
 * Returns the sum of the last three elements of the given vector of pointers to
 * integers.
 * Hint: is this function doing anything more or less than that description?
 */
int vector_sum_pointers_last_three(size_t sz, int *vector_pointers[]) {
    unsigned int sum;
    size_t i;
    edit_thing(vector_pointers, sz - 2);
    sum = 0;
    for (i = sz - 4; i < sz - 1; i++)
        sum += get_next_element(vector_pointers, i);
    return sum;
}

/*
 * Sums the last three entries of the entered vector.
 */
int main(int argc, char **argv) {
    int vector[argc - 1];
    int *vector_pointers[argc - 1];
    unsigned int sum;
    size_t i;

    if (argc < 2) {
        printf("Usage: ./sum [arg1] [arg2] ... [argn]\n");
        return 1;
    }

    if (argc < 5) {
        fprintf(stderr, "Error: you need an array with at least 4 elements\n");
        return 1;
    }

    for (i = 1; i < argc; i++) {
        vector[i - 1] = atoi(argv[i]);
        vector_pointers[i - 1] = &vector[i - 1];
    }

    sum = vector_sum_pointers_last_three(argc - 1, vector_pointers);
    printf("Vector sum is %i.\n", sum);

    return 0;
}
