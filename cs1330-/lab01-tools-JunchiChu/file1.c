#include <stdio.h>
#include <string.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/*
 * Merges two sorted strings that are adjacent to each other in the string str.
 * For example, consider the following string:
 * AQZBCVa321q
 * One might call merge("AQZBCVa321q", 0, 3, 6)
 * because A, at index 0, is the beginning of the first sorted string
 * B, the beginning of the second sorted string, is at index 3,
 * and the last element of the second sorted string is at index 5, so we pass in
 * 6 because our ending index is exclusive.
 * After merge is done, the input string should read
 * "ABCQVZa321q"
 *
 * Hint: Hmmm... looks like the debugging elves in the Sunlab have already done
 * a pretty thorough job with this one. It looks like your problems lie
 * elsewhere... (Translation: The CS033 TAs guarantee that this function works
 * exactly as described above.)
 */
void merge(char *str, char *scratch, int first_idx, int second_idx, int end) {
    int store_idx, orig_second_idx, i;
    char min;

    store_idx = first_idx;
    orig_second_idx = second_idx;
    i = store_idx;
    while (store_idx != end) {
        if (first_idx >= orig_second_idx) {
            // We have already reached the end of the first substring.
            min = str[second_idx];
        } else if (second_idx >= end) {
            // We have already reached the end of the second substring.
            min = str[first_idx];
        } else {
            min = MIN(str[first_idx], str[second_idx]);
        }
        if (min == str[first_idx] && first_idx < orig_second_idx) {
            scratch[store_idx] = min;
            first_idx++;
        } else if (min == str[second_idx] && second_idx < end) {
            scratch[store_idx] = min;
            second_idx++;
        } else {
            fprintf(stderr,
                    "Warning: merge() has internally inconsistent \
                indices.\n");
        }
        store_idx++;
    }
    for (; i < end; ++i) {
        str[i] = scratch[i];
    }
}
/*
 * Implements mergesort on a substring of the given string.
 * Recall the following pseudocode for mergesort:
 function merge_sort(list m)
    // Base case. A list of zero or one elements is sorted, by definition.
    if length(m) <= 1
        return m

    // Recursive case. First, *divide* the list into
    // equal-sized sublists.
    var list left, right
    var integer middle = length(m) / 2
    for each x in m before middle
        add x to left
    for each x in m after or equal    middle
        add x to right

    // Recursively sort both sublists.
    left =    merge_sort(left)
    right =    merge_sort(right)
    // *Conquer*: merge the now-sorted sublists.
    return merge(left, right)
 *
 * The above pseudocode is written in a functional style, where the input to the
 * mergesort function is a list and the output is another list that contains the
 * same elements as the first list, but has been sorted. In C, most mergesort
 * implementations instead use an "in place" mergesort. In this version, the
 * string used as input is mutated so that its elements are sorted. Because this
 * helper takes as input two indices, you should only sort elements between
 * index low (inclusive) and high (exclusive). Some possible helper functions
 * (though certainly not the only possible ones) are given skeletons above.
 *
 * Hint: If this function is crashing due to segmentation faults when accessing
 * very high memory addresses, ask yourself why a recursive algorithm would use
 * up all of a system's available memory.
 */
void mergesort_helper(char *str, char *scratch, int low, int high) {
    int middle;
    if (low < high ){
      middle = (high - low) / 2 + low;
      mergesort_helper(str, scratch, low, middle);
      mergesort_helper(str, scratch, middle, high);
      merge(str, scratch, low, middle, high);
    }
    }

/*
 * Sorts str. Sorting is defined numerically. Every character in a string has a
 * numerical value, so each string should be sorted so its characters are in
 * numerically ascending order. For alphanumeric strings, this will mean that
 * numbers will be sorted first, followed by capital letters, followed by
 * lowercase letters. Therefore MyString321 sorts to 123MSginrty. See
 * http://www.asciitable.com/ for a table of numeric values (called ASCII
 * values) that characters have.
 */
void mergesort(char *str, char *scratch) {
    mergesort_helper(str, scratch, 0, strlen(str));
}

/*
 * Sort the first argument to the program. That will be stored in argv[1].
 * Prints out the result of the sorting. Recall that argv[0] is the name of the
 * program, not the first argument.
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: mergesort <string>\n");
        return 1;
    } else {
        char scratch[strlen(argv[1])];
        mergesort(argv[1], scratch);
        printf("%s\n", argv[1]);
        return 0;
    }
}
