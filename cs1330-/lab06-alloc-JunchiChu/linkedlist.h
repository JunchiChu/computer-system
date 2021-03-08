#ifndef linkedlist_H
#define linkedlist_H

// Declare an opaque struct (defined in linkedlist.c)
typedef struct LinkedList LinkedList;

/******************************
 * Creation/deletion functions
 ******************************/

/* Create a new linkedlist with the given element functions and return it */
LinkedList *linkedlist_new(int (*equals)(void *, void *), void *(*copy)(void *),
                           void (*delete)(void *));

/* Delete the given linked list */
void linkedlist_delete(LinkedList *linkedlist);

/******************************
 * Access/modification functions
 ******************************/

/* Add a copy of the given element to the tail of the list */
void linkedlist_append(LinkedList *linkedlist, void *element);

/*
 * Insert a copy of the given element at the given index (before the element
 * that currently has that index).
 * Inserting at size is equivalent to appending.
 * Return 1 if the element was added successfully
 * 0 otherwise (if the index is invalid)
 */
int linkedlist_insert(LinkedList *linkedlist, void *element, int index);

/* Return 1 if the given element is in the list
 * 0 otherwise
 */
int linkedlist_contains(LinkedList *linkedlist, void *element);

/* Remove the first ocurrence of the given element from the list
 * Return 1 if the element was removed successfully
 * 0 otherwise (if the element was not found)
 */
int linkedlist_remove(LinkedList *linkedlist, void *element);

/******************************
 * Other utility functions
 ******************************/

/* Get the size of the given list */
int linkedlist_size(LinkedList *linkedlist);

/* Print a representation of the linked list,
 * using the given function to print each
 * element
 */
void linkedlist_print(LinkedList *linkedlist, FILE *stream,
                      void (*print_element)(FILE *, void *));

#endif
