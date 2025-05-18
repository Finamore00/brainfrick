#ifndef SCOPE_STACK_H
#define SCOPE_STACK_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    uint64_t *elems;
    size_t len;
    size_t cap;
} ProgramStack;

ProgramStack *stack_Create(size_t cap);
__attribute__((nonnull(1)))void stack_Push(ProgramStack *st, uint64_t elem);
__attribute__((nonnull(1)))uint64_t stack_Peek(ProgramStack *st);
__attribute__((nonnull(1)))uint64_t stack_Pop(ProgramStack *st);
__attribute__((nonnull(1)))bool stack_IsEmpty(ProgramStack *st);
__attribute__((nonnull(1)))void stack_Destroy(ProgramStack *st);

#endif