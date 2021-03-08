#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Element in a linked list
typedef struct list_ele {
    int val;
    struct list_ele *next;
  pthread_mutex_t lock; 
} list_ele_t;

// Head of the linked list
list_ele_t head = {-1, 0, PTHREAD_MUTEX_INITIALIZER};

// Other declarations
pthread_barrier_t bar;
int niterations;


void *seqListManip(void *arg);
void *randomListManip(void *arg);
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// Parses command line arguments and launches threads
int main(int argc, char *argv[]) {
    int error;
    uintptr_t i;





    //
    //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    //pthread_mutex_init(mutex,0);


    
    if (argc != 3) {
        fprintf(stderr, "error: usage: %s threads iterations\n", argv[0]);
        exit(1);
    }

    int nthreads = atoi(argv[1]);
    niterations = atoi(argv[2]);

    pthread_t threads[nthreads];

    if ((error = pthread_barrier_init(&bar, 0, nthreads)) != 0) {
        fprintf(stderr, "pthread_barrier_init: %s\n", strerror(error));
        exit(1);
    }

    for (i = 0; i < nthreads; i++) {
#ifdef SEQUENTIAL
        if ((error = pthread_create(
                 &threads[i], 0, (void *(*)(void *))seqListManip, (void *)i)) !=
            0) {
#else
        if ((error = pthread_create(&threads[i], 0,
                                    (void *(*)(void *))randomListManip,
                                    (void *)i)) != 0) {
#endif
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            exit(1);
        }
    }

    for (i = 0; i < nthreads; i++) {
        pthread_join(threads[i], 0);
    }

    list_ele_t *ele;

    // Prints out the final contents of the list.
    for (ele = &head; ele != 0; ele = ele->next) {
        printf("%d\n", ele->val);
    }

    list_ele_t *tmp;
    for (ele = head.next; ele; ele = tmp) {
        tmp = ele->next;
        free(ele);
    }

    return 0;
}

void insertList(int val);
void deleteList(int val);

/**
 * This function is run by threads in ./sequential.
 * You should not modify it.
 * arg - the argument passed to the new thread (the thread's index in this case)
 */
void *seqListManip(void *arg) {
    // The argument is used to choose the first value that's inserted.
    // The intent is to ensure that the inserted values overlap.
    uintptr_t start = (uintptr_t)arg * niterations / 2;

    // The barrier ensures that all threads start at the same time.
    pthread_barrier_wait(&bar);

    for (int i = start; i < (start + niterations); i++) {
        insertList(i);
    }
    return 0;
}

/**
 * This function is run by threads in ./random.
 * You should not modify it.
 * arg - the argument passed to the new thread (the thread's index in this case)
 */
void *randomListManip(void *arg) {
    // The argument seeds the random number generator.
    // The intent is to ensure that each thread does different things.
    uintptr_t seed = (uintptr_t)arg;

    // The barrier ensures that all threads start at the same time.
    pthread_barrier_wait(&bar);

    for (int i = 0; i < niterations; i++) {
        // Uses the thread-safe version of rand.
        // val is in the range [1, 20].
        int rval = rand_r((unsigned *)&seed);
        int val = (rval % 20) + 1;
        if (val <= 10) {
            insertList(i);
        } else {
            deleteList(val - 10);
        }
    }
    return 0;
}

/**
 * Searches the sorted linked list for a node with the given value.
 * val - the value to search for
 * parentp - a pointer to the list_ele_t pointer of the value's parent
 * list_ele_t
 */
list_ele_t *search(int val, list_ele_t **parentp) {
    /* TODO: edit for fine-grained locking. Note that 
    *    (1) search() should be called with head locked (you will need to make this edit in the calling functions)
    *    (2) search() should return with parent and the found item (if any) locked.
    */
    list_ele_t *par = &head;
    list_ele_t *ele = head.next;
    
 
    while (ele != 0) {
          pthread_mutex_lock(&ele->lock);

        // par is locked here.
        if (val <= ele->val) {
            *parentp = par;
            if (val == ele->val) {
              
	     
                // ele should stay locked.
                return ele;
            } else {

	      pthread_mutex_unlock(&ele->lock);

                return 0;
            }
        }
	pthread_mutex_unlock(&par->lock);
        par = ele;
        ele = ele->next;
       
    }
       

    *parentp = par;
    //    pthread_mutex_unlock(&par->lock);


  
    return 0;
}

/**
 * Inserts an element into the sorted list if it is not already in the list.
 * val - the value to insert
 */
void insertList(int val) {
    // TODO: edit for fine-grained locking
    list_ele_t *found;
    list_ele_t *parent;
    list_ele_t *newItem;
    pthread_mutex_lock(&head.lock);

       
       fprintf(stderr, "adding %d\n", val);

    if ((found = search(val, &parent)) != 0) {
      
        // val is already in the list.
      pthread_mutex_unlock(&found->lock);
      pthread_mutex_unlock(&parent->lock); 
        return;
    }

    // val is not in the list. Adds it.
    if ((newItem = (list_ele_t *)malloc(sizeof(list_ele_t))) == 0) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    pthread_mutex_init(&newItem->lock,0);

    newItem->val = val;
    newItem->next = parent->next;
    parent->next = newItem;
    pthread_mutex_unlock(&parent->lock);
    // fprintf(stderr, "adding %d\n", val);

    return;
        // val is not in the list. Adds it.
        
    

}

/**
 * Deletes an element from the sorted list.
 * val - the value to remove
 */
void deleteList(int val) {
    // TODO: edit for fine-grained locking

    list_ele_t *parent;
    list_ele_t *oldItem;
   pthread_mutex_lock(&head.lock);
  
      fprintf(stderr, "deleting %d\n", val);

    if ((oldItem = search(val, &parent)) == 0) {
        // val is not in the list.
      pthread_mutex_unlock(&parent->lock);
      

        return;
    }

    // val is in the list. Removes it.
    parent->next = oldItem->next;


     pthread_mutex_unlock(&oldItem->lock);
    pthread_mutex_destroy(&oldItem->lock);
    pthread_mutex_unlock(&parent->lock);
    free(oldItem);
    return;
    //pthread_mutex_unlock(&oldItem->lock);
    //  pthread_mutex_destroy(&oldItem->lock);
    // fprintf(stderr, "deleting %d\n", val);

}

 
