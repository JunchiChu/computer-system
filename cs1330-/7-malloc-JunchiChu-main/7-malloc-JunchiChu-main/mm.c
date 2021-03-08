#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * BEFORE GETTING STARTED:
 *
 * Familiarize yourself with the functions and constants/variables
 * in the following included files.
 * This will make the project a LOT easier as you go!!
 *
 * The diagram in Section 3.1 (Specification) of the handout will help you
 * understand the constants in mm.h
 * Section 3.2 (Support Routines) of the handout has information about
 * the functions in mminline.h and memlib.h
 */
#include "./memlib.h"
#include "./mm.h"
#include "./mminline.h"
block_t *epilogue;
block_t *prologue;
// rounds up to the nearest multiple of WORD_SIZE
block_t *extension_heap(size_t m);
block_t *slice(block_t *a, size_t b, size_t c);
void coalesce(block_t *p);
static inline size_t align(size_t size) {
    return (((size) + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1));
}

/*
 *                             _       _ _
 *     _ __ ___  _ __ ___     (_)_ __ (_) |_
 *    | '_ ` _ \| '_ ` _ \    | | '_ \| | __|
 *    | | | | | | | | | | |   | | | | | | |_
 *    |_| |_| |_|_| |_| |_|___|_|_| |_|_|\__|
 *                       |_____|
 *
 * initializes the dynamic storage allocator (allocate initial heap space)
 * arguments: none
 * returns: 0, if successful
 *         -1, if an error occurs
 */
int mm_init(void) {
    flist_first = NULL;
    // allocate space for prologue
    if ((prologue = mem_sbrk(TAGS_SIZE * 2)) == (void *)-1) {
        perror("prologue init error");
        return -1;
    }

    // set the prologue with tag size
    block_set_size_and_allocated(prologue, TAGS_SIZE, 1);
    // Also set up the epilogue as well
    epilogue = block_next(prologue);
    block_set_size_and_allocated(epilogue, TAGS_SIZE, 1);

    return 0;
}

/*     _ __ ___  _ __ ___      _ __ ___   __ _| | | ___   ___
 *    | '_ ` _ \| '_ ` _ \    | '_ ` _ \ / _` | | |/ _ \ / __|
 *    | | | | | | | | | | |   | | | | | | (_| | | | (_) | (__
 *    |_| |_| |_|_| |_| |_|___|_| |_| |_|\__,_|_|_|\___/ \___|
 *                       |_____|
 *
 * allocates a block of memory and returns a pointer to that block's payload
 * arguments: size: the desired payload size for the block
 * returns: a pointer to the newly-allocated block's payload (whose size
 *          is a multiple of ALIGNMENT), or NULL if an error occurred
 */
void *mm_malloc(size_t size) {
    // TODO
    // if size is 0, just return null
    if (size == 0) {
        return NULL;
    }
    // we need to align the size to 8 multiples
    size = align(size);
    // get the global pointer of free list which points to the head of free list
    block_t *cur = flist_first;
    size_t ask_size = size + TAGS_SIZE;
    // request size cannot less then miniblock bound
    if (ask_size < MINBLOCKSIZE) {
        ask_size = MINBLOCKSIZE;
    }
    int not_fb = 0;
    // if not_fb is 0, then there is free block available, we use a while loop
    // to iterate the free list to find out if there is a candidate
    while (cur != NULL && block_size(cur) < ask_size) {
        cur = block_flink(cur);

        if (cur == flist_first) {
            // not_fb is 1, we iterate back to list, there is no free block
            // available
            not_fb = 1;
            break;
        }
    }
    // cur is a free block available
    if (cur != NULL && not_fb == 0) {
        pull_free_block(cur);
        // tmp is the gap size between the request size and free block size
        size_t tmp = block_size(cur) - ask_size;
        // if tmp is smaller than minimum, this space will be wasted, just
        // return payload
        if (tmp < MINBLOCKSIZE) {
            block_set_allocated(cur, 1);
            return cur->payload;
        }

        block_set_size_and_allocated(cur, ask_size, 1);
        // tmp is not 0, we can set the leftover space unallocated
        if (tmp != 0) {
            block_set_size_and_allocated(block_next(cur), tmp, 0);
        }
        // if it is big enough to be inserted into free list, go for it.
        if (tmp >= MINBLOCKSIZE) {
            insert_free_block(block_next(cur));
        }

        return cur->payload;
    }
    // if the function cannot be returned, that means free list cannot satisfied
    // us. We need to extend heap.
    block_t *newblock = extension_heap(ask_size);
    // newblock will be the block extended, we need to connect it with the
    // following order: prevblock -> newblock -> epilogue
    block_t *prevblock = block_prev(epilogue);
    newblock = block_next(prevblock);
    block_set_size_and_allocated(newblock, ask_size, 1);
    epilogue = block_next(newblock);
    block_set_size_and_allocated(epilogue, TAGS_SIZE, 1);

    return newblock->payload;
}

/*                              __
 *     _ __ ___  _ __ ___      / _|_ __ ___  ___
 *    | '_ ` _ \| '_ ` _ \    | |_| '__/ _ \/ _ \
 *    | | | | | | | | | | |   |  _| | |  __/  __/
 *    |_| |_| |_|_| |_| |_|___|_| |_|  \___|\___|
 *                       |_____|
 *
 * frees a block of memory, enabling it to be reused later
 * arguments: ptr: pointer to the block's payload
 * returns: nothing
 */
void mm_free(void *ptr) {
    // null ptr should not free anything
    if (ptr == NULL) {
        return;
    }
    // use payload_ti_block to get the block we need to free
    block_t *free_block = payload_to_block(ptr);
    // if it is freed, not need to free again
    if (!block_allocated(free_block)) {
        return;
    }
    // to free a block just set allocated to 0 and insert it to the list
    block_set_allocated(free_block, 0);
    insert_free_block(free_block);
    // it is possible to have lots of tiny blocks, so we have to coalesce.
    coalesce(free_block);
    return;
}

/*
 *                                            _ _
 *     _ __ ___  _ __ ___      _ __ ___  __ _| | | ___   ___
 *    | '_ ` _ \| '_ ` _ \    | '__/ _ \/ _` | | |/ _ \ / __|
 *    | | | | | | | | | | |   | | |  __/ (_| | | | (_) | (__
 *    |_| |_| |_|_| |_| |_|___|_|  \___|\__,_|_|_|\___/ \___|
 *                       |_____|
 *
 * reallocates a memory block to update it with a new given size
 * arguments: ptr: a pointer to the memory block's payload
 *            size: the desired new payload size
 * returns: a pointer to the new memory block's payload
 */
void *mm_realloc(void *ptr, size_t size) {
    // TODO
    // these are speicial cases
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }
    if (ptr == NULL) {
        mm_malloc(size);
    }
    // ask size is requested size. ori size is the original size of the ptr. pay
    // size is the size that if we call memmove or memcpy that amount need to be
    // copied
    size_t ask_size = align(size) + TAGS_SIZE;
    block_t *cur = payload_to_block(ptr);
    size_t ori_size = block_size(cur);
    size_t pay_size = ori_size - TAGS_SIZE;
    if (ask_size <= ori_size) {
        // It is possible to combine with the next block if the next block is
        // unallocated, but doing that lower my performance. So I just return the
        // payload.
        return cur->payload;

    } else {
        // set up the cur/pre/next pointer if requested size is bigger than
        // original
        block_t *cur = payload_to_block(ptr);
        block_t *pre = block_prev(cur);
        block_t *next = block_next(cur);

        // if pre_serve/next_serve is 1, it is okay to combine with the previous
        // block/next block
        int pre_serve = 0;
        int next_serve = 0;
        if (block_size(pre) != 16 && !block_allocated(pre)) {
            pre_serve = 1;
        }

        if (block_size(next) != 16 && !block_allocated(next)) {
            next_serve = 1;
        }
        // case 1: if combine with the previous block, check if it big enough
        // for new requested size

        if ((block_size(cur) + block_size(pre)) >= ask_size) {
            if (pre_serve == 1) {
                // if prev is available, pull it off from free list
                pull_free_block(pre);
                // set it up
                block_set_size_and_allocated(
                    pre, block_size(cur) + block_size(pre), 1);
                cur = pre;
                // copy the payload, since we need to move the memory to the
                // current pointer
                memmove(cur->payload, ptr, pay_size);

                return cur->payload;
            }
        }
        // case2: if combine with next is big enough for new requested size
        if ((block_size(cur) + block_size(next)) >= ask_size) {
            if (next_serve == 1) {
                // if the next block is available, set it up
                pull_free_block(next);
                block_set_size_and_allocated(
                    cur, block_size(cur) + block_size(next), 1);
                // there is no need to copy ptr payload because cur is where the
                // ptr refers to, just return payload is fine

                return cur->payload;
            }
        }

        // case3: only combine prev or next cannot satisfy, must combine both
        if ((block_size(cur) + block_size(pre) + block_size(next)) >=
            ask_size) {
            if (next_serve == 1 && pre_serve == 1) {
                // pull the both prev and next and set them up
                pull_free_block(next);
                pull_free_block(pre);
                block_set_size_and_allocated(
                    pre, block_size(cur) + block_size(next) + block_size(pre),
                    1);
                // move the pointer to cur
                cur = pre;
                // now cur pointer is pre, you need to copy payload.
                memmove(cur->payload, ptr, pay_size);

                return cur->payload;
            }
        }

        // case 4: if combine with prev and next cannot help, check out the free
        // list to see if they can help

        int check_free_list = 0;
        if (check_free_list == 0) {
            cur = flist_first;
            int not_fb = 0;
            // the same code in malloc, iterate the free list to see if there is
            // one available for requested size
            while (cur != NULL && block_size(cur) < ask_size) {
                cur = block_flink(cur);

                if (cur == flist_first) {
                    // not_fb is 1, then there is no such candidate for new
                    // requested size
                    not_fb = 1;
                    break;
                }
            }

            if (cur != NULL && not_fb == 0) {
                // there is a candidate for new requested size, pull it off from
                // the free list
                pull_free_block(cur);

                size_t tmp = block_size(cur) - ask_size;

                if (tmp < MINBLOCKSIZE) {
                    block_set_allocated(cur, 1);
                    // since we use a free list, the ptr will be lost, we need
                    // to copy the payload and free the ptr
                    memmove(cur->payload, ptr, pay_size);
                    mm_free(ptr);
                    return cur->payload;
                }

                block_set_size_and_allocated(cur, ask_size, 1);
                if (tmp != 0) {
                    // handle the edge case when the gap size is not 0
                    block_set_size_and_allocated(block_next(cur), tmp, 0);
                }
                if (tmp >= MINBLOCKSIZE) {
                    // new gap is big enough to be inserted into free list
                    insert_free_block(block_next(cur));
                }
                // since we use a free list, the ptr will be lost, we need to
                // copy the payload and free the ptr

                memmove(cur->payload, ptr, pay_size);
                mm_free(ptr);
                return cur->payload;
            }
        }
        // case5: after done with case 1,2,3,4(check prev and next unallocated
        // block, and check the free list), we still cannot find the right
        // candidate. We must extend the heap.
        int not_enough_even_coalboth = 0;

        if (not_enough_even_coalboth == 0) {
            // just malloc the request size and we need to copy the payload to
            // the next block newbl, because ptr will be lost and freed.
            void *newbl = mm_malloc(ask_size);
            memmove(newbl, ptr, pay_size);
            mm_free(ptr);
            return newbl;
        }
    }
    return ptr;
}
/*
A simple helper function, if not free list member can help, just extend the heap
by calling mem sbrk
 */
block_t *extension_heap(size_t size) {
    block_t *newblock;

    if ((newblock = mem_sbrk(size)) == (void *)-1) {
        perror("extensionnn heap error");
        return NULL;
    }

    return newblock;
}
/*
This slice function is my plan one, so when we need to extend, we extend a huge
heap, but I realize it is lowing my util. So I change my plan, just extend
whatever they ask for. Even this is function is not called, but I keep it here.
 */
block_t *slice(block_t *fre_blk, size_t size, size_t need_size) {
    size_t gap;
    if (size > need_size) {
        gap = size - need_size;
    } else {
        block_set_size_and_allocated(fre_blk, need_size, 1);
        return fre_blk;
    }
    if ((gap) < MINBLOCKSIZE) {
        return fre_blk;
    }
    pull_free_block(fre_blk);

    block_set_size_and_allocated(fre_blk, need_size, 1);
    block_t *leftover = block_next(fre_blk);
    block_set_size_and_allocated(leftover, gap, 0);
    insert_free_block(leftover);
    epilogue = block_next(leftover);

    return fre_blk;
}
/*
coalesce is a helper function to see whether for a free block to combine with
adjacent blocks. It takes one parameter which is the block that will be freed.
 */
void coalesce(block_t *b) {
    // get the prev and next block for avaible candidate
    block_t *nex = block_next(b);
    block_t *prev = block_prev(b);
    // if prev_block_combined is 1, then the block b can combine with the prev.
    int prev_block_combined = 0;
    pull_free_block(b);
    if (!block_allocated(prev)) {
        // if prev is available, pull it off. Set it up by using
        // block_set_size_and_allocated and then set the indicator
        // prev_block_combined to 1
        pull_free_block(prev);
        size_t prev_cur = block_size(prev) + block_size(b);
        block_set_size_and_allocated(prev, prev_cur, 0);
        prev_block_combined = 1;
    }
    if (!block_allocated(nex)) {
        // if the next block is available, we check if the prev block has
        // joined. The reason for this if condition is that if the prev is
        // combined, our current pointer is pointing to prev, not b. If it is not
        // the case, then b is the current pointer.
        if (prev_block_combined == 1) {
            pull_free_block(nex);
            size_t nex_cur = block_size(nex) + block_size(prev);
            // note here prev is the current pointer
            block_set_size_and_allocated(prev, nex_cur, 0);
            insert_free_block(prev);
            return;

        } else {
            pull_free_block(nex);
            size_t nex_cur = block_size(nex) + block_size(b);
            // note here b is the current pointer
            block_set_size_and_allocated(b, nex_cur, 0);
            insert_free_block(b);
            return;
        }
    }
    // depends on which case, you decide which pointer is going to be inserted
    // in the free list
    if (prev_block_combined == 1) {
        insert_free_block(prev);
    } else {
        insert_free_block(b);
    }
}
