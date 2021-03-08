#include <stdio.h>
#include <stdlib.h>

#include "./linkedlist.h"
#include "./string_fns.h"
/* A ListNode is a node in a singly-linked list */
typedef struct ListNode ListNode;
struct ListNode {
    void *value;
    ListNode *next;
};

struct LinkedList {
    // Function to compare two elements for equality
    int (*equals)(void *, void *);
    // Function that copies an element and returns a pointer to the copy.
    void *(*copy)(void *);
    // Function to delete an element
    void (*delete)(void *);

    ListNode *head;  // The first element (remember to update)
    int size;        // The number of elements in the list (remember to update)
};

/******************************
 * Creation/deletion functions
 ******************************/

/* Create a new linkedlist with the given element functions and return it */
LinkedList *linkedlist_new(int (*equals)(void *, void *), void *(*copy)(void *),
                           void (*delete)(void *)) {
    // TODO: 1. Allocate space for a linkedlist struct.
    //       2. Initialize the struct.
  struct LinkedList *lklist= malloc(sizeof(struct LinkedList));
  lklist->head = NULL;
  lklist-> size = 0;
  lklist -> equals=string_equals;
  lklist -> copy = string_copy;
  lklist -> delete = string_delete;
  
    return lklist;
}

/* Delete the given linked list */
void linkedlist_delete(LinkedList *linkedlist) {
    // TODO: 1. Delete all of the linked list's internal data.

  //       2. Free the struct.
  ListNode * h = linkedlist->head;
  ListNode *fir;
  while(h!=NULL){
    linkedlist->delete(h->value);
    fir = h;
    h =  h->next;
    free(fir);
    
  }
   
  linkedlist->size =0;
 
  free(linkedlist);
}

/******************************
 * Access/modification functions
 ******************************/

/* Add a copy of the given element to the tail of the list */
void linkedlist_append(LinkedList *linkedlist, void *element) {
    // TODO: 1. Find the last node in the linked list.
    //       2. Create a copy of the element and store the copy in
    //          a new list node.
    //       3. Set the next pointer of the last node to the newly
    //          created node.
  ListNode * cur = linkedlist->head;
  ListNode *new =(struct ListNode*)malloc(sizeof(struct ListNode));
  new -> value = linkedlist->copy(element);
  new -> next =NULL;
  if(cur==NULL){
    linkedlist->head = new;
    linkedlist->size++;
    return;
  }
  while(cur->next!=NULL){
    cur = cur ->next;
  }
  cur->next = new;
  linkedlist->size++;
    
  
}

/* Insert a copy of the given element at the given index (before the element
 * that currently has that index).
 * Inserting at size is equivalent to appending.
 * Return 1 if the element was added successfully
 * 0 otherwise (if the index is invalid)
 */
int linkedlist_insert(LinkedList *linkedlist, void *element, int index) {
    // TODO: 1. Find the node at the given index, if such a node exists.
    //       2. Create a copy of the element and store the copy in
    //          a new list node.
    //       3. Update the next pointers of the old and new nodes.
 ListNode * cur = linkedlist->head;
 if(index ==0){
   
 ListNode * new0 =  (struct ListNode*)malloc(sizeof(struct ListNode));
 new0 -> value =linkedlist->copy(element);
 new0 -> next = cur;
 linkedlist->head = new0;
 linkedlist->size ++;
 return 1;
 }
  int i =0;
  while(i!=index-1){
    if(cur ==NULL){
      return 0;
    }
    cur = cur -> next;
    i++;
  }
  ListNode * curnext = cur -> next;
  ListNode * new =  (struct ListNode*)malloc(sizeof(struct ListNode));
  
  new -> value =linkedlist->copy(element);
  new -> next = curnext;
  cur -> next = new;
  linkedlist->size++;
  
    return 1;
}

/* Return 1 if the given element is in the list
 * 0 otherwise
 */
int linkedlist_contains(LinkedList *linkedlist, void *element) {
    ListNode *current = linkedlist->head;
    while (current != NULL) {
        if (linkedlist->equals(current->value, element)) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

/* Remove the first occurence of the given element from the list
 * Return 1 if the element was removed successfully
 * 0 otherwise (if the element was not found)
 */
int linkedlist_remove(LinkedList *linkedlist, void *element) {
    // TODO: 1. Find the first node containing the element.
    //       2. If an element is found, delete the linkedlist's copy
    //          of the element and remove the node.
    //       3. Update the next pointer of the previous element.
   ListNode *current = linkedlist->head;
   ListNode *prev = NULL;
   if(current!=NULL && linkedlist->equals(current->value,element)){
     linkedlist->delete(current->value);
     linkedlist->head = linkedlist->head -> next ;
     linkedlist->size--;
     if(linkedlist->size==0){
       struct ListNode* header ;

       header = (struct ListNode*)malloc(sizeof(struct ListNode));
  header->value = NULL;
  header->next = NULL;

  linkedlist->head = header;
     }
     free(current);
     return 1;
   }
   while(current!=NULL){
     if(linkedlist->equals(current->value, element)){
     
       linkedlist->delete(current->value);
       prev->next = current -> next;
       free(current);
       linkedlist->size--;
       return 1;
     }
     prev = current;
     current = current -> next;
     }
    return 0;
}

/******************************
 * Other utility functions
 ******************************/

/* Get the size of the given set */
int linkedlist_size(LinkedList *linkedlist) { return linkedlist->size; }

/* Print a representation of the linkedlist,
 * using the given function to print each
 * element
 */
void linkedlist_print(LinkedList *linkedlist, FILE *stream,
                      void (*print_element)(FILE *, void *)) {
    fprintf(stream, "{size=%d} ", linkedlist->size);
    ListNode *bn = linkedlist->head;
    fprintf(stream, "[");
    while (bn) {
        print_element(stream, bn->value);
        bn = bn->next;
        if (bn) {
            fprintf(stream, ", ");
        }
    }
    fprintf(stream, "]");
}
