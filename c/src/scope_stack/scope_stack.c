#include "scope_stack.h"

ProgramStack *stack_Create(size_t cap) {
    ProgramStack *new_instance = malloc(sizeof(ProgramStack));
    if (new_instance == NULL) {
        exit(1);
    }

    uint64_t *elems = malloc(sizeof(uint64_t) * cap);
    if (elems == NULL) {
        free(new_instance);
        exit(1);
    }

    *new_instance = (ProgramStack) {
        .elems = elems,
        .len = 0,
        .cap = cap
    };

    return new_instance;
}

bool stack_IsEmpty(ProgramStack *st) {
    return st->len <= 0;
}

void stack_Push(ProgramStack *st, uint64_t elem) {
    if (st->len >= st->cap) {
        st->cap *= 2;
        st->elems = realloc(st->elems, sizeof(uint64_t) * st->cap);
    }

    st->elems[st->len++] = elem;
    return;
}

uint64_t stack_Peek(ProgramStack *st) {
    if (stack_IsEmpty(st)) {
        return UINT64_MAX;
    }

    return st->elems[st->len - 1];
}

uint64_t stack_Pop(ProgramStack *st) {
    if (stack_IsEmpty(st)) {
        return UINT64_MAX;
    }

    return st->elems[--st->len];
}

void stack_Destroy(ProgramStack *st) {
    if (st->elems != NULL) free(st->elems);
    st->elems = NULL;
    free(st);
    return;
}