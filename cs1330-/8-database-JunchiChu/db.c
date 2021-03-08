

#include "./db.h"
#include <assert.h>
#include <comm.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 256
// enum locktype {l_read, l_write};

// The root node of the binary tree, unlike all
// other nodes in the tree, this one is never
// freed (it's allocated in the data region).
node_t head = {"", "", 0, 0, PTHREAD_RWLOCK_INITIALIZER};
// void locker(locktype a,pthread_rwlock_t* b);

void locker(locktype lt, pthread_rwlock_t *lock) {
    int res;

    if (lt == l_write) {
        if ((res = pthread_rwlock_wrlock(lock))) {
            handle_error_en(res, "fail lock a");
        }
    } else {
        if ((res = pthread_rwlock_rdlock(lock))) {
            handle_error_en(res, "fail lock b");
        }
    }
}

node_t *node_constructor(char *arg_name, char *arg_value, node_t *arg_left,
                         node_t *arg_right) {
    size_t name_len = strlen(arg_name);
    size_t val_len = strlen(arg_value);

    if (name_len > MAXLEN || val_len > MAXLEN) return 0;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    if (new_node == 0) return 0;

    if ((new_node->name = (char *)malloc(name_len + 1)) == 0) {
        free(new_node);
        return 0;
    }

    if ((new_node->value = (char *)malloc(val_len + 1)) == 0) {
        free(new_node->name);
        free(new_node);
        return 0;
    }

    if ((snprintf(new_node->name, MAXLEN, "%s", arg_name)) < 0) {
        free(new_node->value);
        free(new_node->name);
        free(new_node);
        return 0;
    } else if ((snprintf(new_node->value, MAXLEN, "%s", arg_value)) < 0) {
        free(new_node->value);
        free(new_node->name);
        free(new_node);
        return 0;
    }

    new_node->lchild = arg_left;
    new_node->rchild = arg_right;
    return new_node;
}

void node_destructor(node_t *node) {
    if (node->name != 0) free(node->name);
    if (node->value != 0) free(node->value);

    free(node);
}

node_t *search(char *, node_t *, node_t **, locktype loc);
void db_query(char *name, char *result, int len) {
    // TODO: Make this thread-safe!
    node_t *target;

    locker(l_read, &head.lock);
    target = search(name, &head, 0, l_read);

    if (target == 0) {
        snprintf(result, len, "not found");
        return;
    } else {
        snprintf(result, len, "%s", target->value);
        int res;
        if ((res = pthread_rwlock_unlock(&target->lock))) {
            handle_error_en(res, "fail lock a");
        }
        return;
    }
}

int db_add(char *name, char *value) {
    // TODO: Make this thread-safe!
    node_t *parent;
    node_t *target;
    node_t *newnode;
    locker(l_write, &head.lock);
    int res;
    if ((target = search(name, &head, &parent, l_write)) != 0) {
        if ((res = pthread_rwlock_unlock(&target->lock))) {
            handle_error_en(res, "fail lock a");
        }
        if ((res = pthread_rwlock_unlock(&parent->lock))) {
            handle_error_en(res, "fail lock a");
        }
        return (0);
    }

    newnode = node_constructor(name, value, 0, 0);
    if ((res = pthread_rwlock_init(&newnode->lock, 0))) {
        handle_error_en(res, "fail lock a");
        free(newnode);
    }
    if (strcmp(name, parent->name) < 0)
        parent->lchild = newnode;
    else
        parent->rchild = newnode;

    if ((res = pthread_rwlock_unlock(&parent->lock))) {
        handle_error_en(res, "fail lock a");
    }
    return (1);
}

int db_remove(char *name) {
    // TODO: Make this thread-safe!
    node_t *parent;
    node_t *dnode;
    node_t *next;
    int res;

    locker(l_write, &head.lock);

    // first, find the node to be removed
    if ((dnode = search(name, &head, &parent, l_write)) == 0) {
        // it's not there

        if ((res = pthread_rwlock_unlock(&parent->lock))) {
            handle_error_en(res, "fail lock a");
        }

        return (0);
    }

    // We found it, if the node has no
    // right child, then we can merely replace its parent's pointer to
    // it with the node's left child.

    if (dnode->rchild == 0) {
        if (strcmp(dnode->name, parent->name) < 0)
            parent->lchild = dnode->lchild;
        else
            parent->rchild = dnode->lchild;

        // done with dnode

        if ((res = pthread_rwlock_unlock(&dnode->lock))) {
            handle_error_en(res, "fail unlock a");
        }
        if ((res = pthread_rwlock_unlock(&parent->lock))) {
            handle_error_en(res, "fail unlock a");
        }

        node_destructor(dnode);
        if ((res = pthread_rwlock_destroy(&dnode->lock))) {
            handle_error_en(res, "fail lock a");
            free(dnode);
            exit(1);
        }

    } else if (dnode->lchild == 0) {
        // ditto if the node had no left child
        if (strcmp(dnode->name, parent->name) < 0)
            parent->lchild = dnode->rchild;
        else
            parent->rchild = dnode->rchild;

        // done with dnode

        if ((res = pthread_rwlock_unlock(&dnode->lock))) {
            handle_error_en(res, "fail lock a");
        }
        if ((res = pthread_rwlock_unlock(&parent->lock))) {
            handle_error_en(res, "fail lock a");
        }

        node_destructor(dnode);
        if ((res = pthread_rwlock_destroy(&dnode->lock))) {
            handle_error_en(res, "fail lock a");
            free(dnode);
            exit(1);
        }
    } else {
        // Find the lexicographically smallest node in the right subtree and
        // replace the node to be deleted with that node. This new node thus is
        // lexicographically smaller than all nodes in its right subtree, and
        // greater than all nodes in its left subtree
        if ((res = pthread_rwlock_unlock(&parent->lock))) {
            handle_error_en(res, "fail lock a");
        }
        next = dnode->rchild;
        locker(l_write, &next->lock);
        node_t **pnext = &dnode->rchild;

        while (next->lchild != 0) {
            // work our way down the lchild chain, finding the smallest node
            // in the subtree.
            locker(l_write, &next->lchild->lock);
            node_t *nextl = next->lchild;

            pnext = &next->lchild;

            if ((res = pthread_rwlock_unlock(&next->lock))) {
                handle_error_en(res, "fail lock a");
            }
            next = nextl;
        }

        dnode->name = realloc(dnode->name, strlen(next->name) + 1);
        dnode->value = realloc(dnode->value, strlen(next->value) + 1);

        snprintf(dnode->name, MAXLEN, "%s", next->name);
        snprintf(dnode->value, MAXLEN, "%s", next->value);
        *pnext = next->rchild;

        if ((res = pthread_rwlock_unlock(&next->lock))) {
            handle_error_en(res, "fail lock a");
        }

        node_destructor(next);
        if ((res = pthread_rwlock_destroy(&next->lock))) {
            handle_error_en(res, "fail lock a");
        }
        if ((res = pthread_rwlock_unlock(&dnode->lock))) {
            handle_error_en(res, "fail lock a");
        }
    }

    return (1);
}

node_t *search(char *name, node_t *parent, node_t **parentpp, locktype a) {
    // Search the tree, starting at parent, for a node containing
    // name (the "target node").  Return a pointer to the node,
    // if found, otherwise return 0.  If parentpp is not 0, then it points
    // to a location at which the address of the parent of the target node
    // is stored.  If the target node is not found, the location pointed to
    // by parentpp is set to what would be the the address of the parent of
    // the target node, if it were there.
    //
    // TODO: Make this thread-safe!

    node_t *next;
    node_t *result;
    int res;

    if (strcmp(name, parent->name) < 0) {
        next = parent->lchild;
    } else {
        next = parent->rchild;
    }

    if (next == NULL) {
        result = NULL;
    } else {
        locker(a, &next->lock);
        if (strcmp(name, next->name) == 0) {
            result = next;
        } else {
            if ((res = pthread_rwlock_unlock(&parent->lock))) {
                handle_error_en(res, "fail lock a");
            }
            return search(name, next, parentpp, a);
        }
    }

    if (parentpp != NULL) {
        *parentpp = parent;
    } else {
        if ((res = pthread_rwlock_unlock(&parent->lock))) {
            handle_error_en(res, "fail lock a");
        }
    }

    return result;
}

static inline void print_spaces(int lvl, FILE *out) {
    for (int i = 0; i < lvl; i++) {
        fprintf(out, " ");
    }
}

/* helper function for db_print */
void db_print_recurs(node_t *node, int lvl, FILE *out) {
    // TODO: Make this thread-safe!
    // print spaces to differentiate levels
    print_spaces(lvl, out);
    int res;
    // print out the current node
    if (node == NULL) {
        fprintf(out, "(null)\n");
        return;
    }
    locker(l_read, &node->lock);
    if (node == &head) {
        fprintf(out, "(root)\n");
    } else {
        fprintf(out, "%s %s\n", node->name, node->value);
    }

    db_print_recurs(node->lchild, lvl + 1, out);
    db_print_recurs(node->rchild, lvl + 1, out);
    if ((res = pthread_rwlock_unlock(&node->lock))) {
        handle_error_en(res, "fail lock a");
    }
}

int db_print(char *filename) {
    FILE *out;
    if (filename == NULL) {
        db_print_recurs(&head, 0, stdout);
        return 0;
    }

    // skip over leading whitespace
    while (isspace(*filename)) {
        filename++;
    }

    if (*filename == '\0') {
        db_print_recurs(&head, 0, stdout);
        return 0;
    }

    if ((out = fopen(filename, "w+")) == NULL) {
        return -1;
    }

    db_print_recurs(&head, 0, out);
    fclose(out);

    return 0;
}

/* Recursively destroys node and all its children. */
void db_cleanup_recurs(node_t *node) {
    if (node == NULL) {
        return;
    }

    db_cleanup_recurs(node->lchild);
    db_cleanup_recurs(node->rchild);

    node_destructor(node);
    pthread_rwlock_destroy(&node->lock);
}

void db_cleanup() {
    db_cleanup_recurs(head.lchild);
    db_cleanup_recurs(head.rchild);
}

void interpret_command(char *command, char *response, int len) {
    char value[MAXLEN];
    char ibuf[MAXLEN];
    char name[MAXLEN];
    int sscanf_ret;

    if (strlen(command) <= 1) {
        snprintf(response, len, "ill-formed command");
        return;
    }

    // which command is it?
    switch (command[0]) {
        case 'q':
            // Query
            sscanf_ret = sscanf(&command[1], "%255s", name);
            if (sscanf_ret < 1) {
                snprintf(response, len, "ill-formed command");
                return;
            }
            db_query(name, response, len);
            if (strlen(response) == 0) {
                snprintf(response, len, "not found");
            }

            return;

        case 'a':
            // Add to the database
            sscanf_ret = sscanf(&command[1], "%255s %255s", name, value);
            if (sscanf_ret < 2) {
                snprintf(response, len, "ill-formed command");
                return;
            }
            if (db_add(name, value)) {
                snprintf(response, len, "added");
            } else {
                snprintf(response, len, "already in database");
            }

            return;

        case 'd':
            // Delete from the database
            sscanf_ret = sscanf(&command[1], "%255s", name);
            if (sscanf_ret < 1) {
                snprintf(response, len, "ill-formed command");
                return;
            }
            if (db_remove(name)) {
                snprintf(response, len, "removed");
            } else {
                snprintf(response, len, "not in database");
            }

            return;

        case 'f':
            // process the commands in a file (silently)
            snprintf(response, len, "1");
            fprintf(stderr, "why sus");
            sscanf_ret = sscanf(&command[1], "%255s", name);
            if (sscanf_ret < 1) {
                snprintf(response, len, "ill-formed command");
                return;
            }
            snprintf(response, len, "2");

            FILE *finput = fopen(name, "r");
            if (!finput) {
                snprintf(response, len, "bad file name");
                return;
            }
            //	    int x = 12;
            while (fgets(ibuf, sizeof(ibuf), finput) != 0) {
                pthread_testcancel();  // fgets is not a cancellation point
                interpret_command(ibuf, response, len);
                // fprintf(stderr, "why not retunr %s", ibuf);
                // fprintf(stderr,"why  retunr %s",finput);

                //	if(x==0){
                // break;
                //}
                // x--;
            }
            // fprintf(stderr,"why oky1 23");

            fclose(finput);
            snprintf(response, len, "file processed");
            fprintf(stderr, "why not retunr");
            return;

        default:
            snprintf(response, len, "ill-formed command");
            return;
    }
}
