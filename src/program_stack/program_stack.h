#ifndef PROGRAM_STACK_H
#define PROGRAM_STACK_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint *items;
    size_t length;
    size_t capacity;
} ProgramStack;

ProgramStack pstack_Create(size_t capacity);
void pstack_Push(ProgramStack *s, uint elem);
uint pstack_Peek(ProgramStack *s);
uint pstack_pop(ProgramStack *s);
void pstack_Destroy(ProgramStack *s);

#endif
