#include "program_stack.h"
#include <limits.h>

/*
 * Implementation of a simple stack for scope tracking
 * in brainf*ck bracket statements. Provides push, peek
 * and pop operations alongside constructor and destructor
 * functions.
 *
 * ProgramStack definition:
 *
 * typedef struct {
 *     uint *items;
 *     size_t length;
 *     size_t capacity;
 * } ProgramStack;
 * */

/**
 * Creates a new ProgramStack instance with the given capacity.
 * */
ProgramStack pstack_Create(size_t capacity) {
    uint *items = calloc(capacity, sizeof(uint));
    if (items == NULL) {
        //If calloc fails something is horribly wrong. Exit.
        exit(1);
    }

    return (ProgramStack) {
        .items = items,
        .length = 0,
        .capacity = capacity
    };
}

/**
 * Pushes an element into the stack. 
 * */
void pstack_Push(ProgramStack *s, uint elem) {
    if (s == NULL) {
        return;
    }

    if (s->length >= s->capacity) {
        s->capacity *= 2;
        s->items = realloc(s->items, sizeof(uint) * s->capacity);
    }

    s->items[s->length++] = elem;
    return;
}

/**
 * Peeks the top element of the stack. In case the stack instance is NULL
 * or the stack is empty an error value of UINT_MAX is returned
 * */
uint pstack_Peek(ProgramStack *s) {
    if (s == NULL || s->length == 0) {
        return UINT_MAX;
    }

    return s->items[s->length - 1];
}

/**
 * Removes the top element from the stack and returns it. In case the stack
 * instance is NULL or the stack is empty an error value of UINT_MAX is
 * returned. */
uint pstack_Pop(ProgramStack *s) {
    if (s == NULL || s->length == 0) {
        return UINT_MAX;
    }

    return s->items[--s->length];
}

/**
 * Cleans up allocated memory for the stack instance.
 * */
void pstack_Destroy(ProgramStack *s) {
    if (s == NULL || s->items == NULL) {
        return;
    }

    free(s->items);
    s->items = NULL;
    return;
}
