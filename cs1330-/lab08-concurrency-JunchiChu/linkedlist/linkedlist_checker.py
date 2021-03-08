import sys
import os
import argparse

description = """
Program that succeeds if the output in f is a proper list, in order, and is \
    consistent with the added/removed statements.
"""


def parse_output_file(ops_file, final_list):
    s = set()
    s.add(-1)
    actual_remaining_elements = []

    # process ops file
    with open(ops_file, 'r') as f:
        while True:
            l = f.readline()
            if not l:
                break
            l = l.split(' ')
            op, n = l[0], int(l[1])
            if op == "adding":
                s.add(n)
            elif op == "deleting":
                if n in s:
                    s.remove(n)
            else:
                print("Unexpected operation type {l}".format(l=l))
                sys.exit(1)

    # this file should contain final state of the linked list
    with open(final_list, 'r') as f:
        while True:
            l = f.readline()
            if not l:
                break
            actual_remaining_elements.append(int(l))

    expected_remaining_elements = sorted(s)
    return [expected_remaining_elements, actual_remaining_elements]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description)
    parser.add_argument('--ops_file',
                        action='store', dest='ops_file', required=True,
                        help='stdout file of ./random or ./sequential')
    parser.add_argument('--final_list',
                        action='store', dest='final_list', required=True,
                        help='stderr file of ./random or ./sequential')

    try:
        results = parser.parse_args()
    except Exception as ex:
        print(str(ex))
        sys.exit(ex.errno)

    [expected_remaining_elements, actual_remaining_elements] = parse_output_file(
        results.ops_file, results.final_list)

    if len(expected_remaining_elements) == len(actual_remaining_elements):
        if all([x == y for (x, y) in zip(expected_remaining_elements, actual_remaining_elements)]):
            sys.exit(0)  # success

    print("Remaining elements in list do not match what was added/deleted!")
    print("Got: ", actual_remaining_elements)
    print("Expected: ", expected_remaining_elements)
    sys.exit(1)  # failure
