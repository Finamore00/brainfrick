#include "src/scope_stack/scope_stack.h"
#include <unistd.h>
#include <stdio.h>

#define MEM_CAP 10000
#define STACK_CAP 10

bool __IsComment(char c);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: Wrong usage.\n");
        fprintf(stderr, "Usage: brainfrick <src_file>\n");
        return EXIT_SUCCESS;
    }

    FILE *src = fopen(argv[1], "r+");
    if (src == NULL) {
        fprintf(stderr, "ERROR: Provided source file non-existent or unreadable.\n");
        return EXIT_FAILURE;
    }

    uint8_t memory[MEM_CAP] = {};
    size_t mem_ptr = 0;
    uint64_t pc = 0;
    ProgramStack *st = stack_Create(STACK_CAP);
    bool err_flag = false;

    for (int c = fgetc(src); c != EOF && !err_flag; c = fgetc(src)) {
        switch ((char) c) {
            case '+':
                memory[mem_ptr] += 1;
                break;
            case '-':
                memory[mem_ptr] -= 1;
                break;
            case '>':
                mem_ptr = mem_ptr < MEM_CAP - 1 ? mem_ptr + 1 : mem_ptr;
                break;
            case '<':
                mem_ptr = mem_ptr > 0 ? mem_ptr - 1 : mem_ptr;
                break;
            case '.':
                printf("%c", (char)memory[mem_ptr]);
                fflush(stdout);
                break;
            case ',':
                fflush(stdin);
                memory[mem_ptr] = (uint8_t)getchar();
                break;
            case '[':
                if (memory[mem_ptr] == 0) {
                    //Look for matching ']'
                    uint64_t depth = 1;
                    for (c = fgetc(src); depth > 0 && c != EOF; c = fgetc(src)) {
                        if ((char)c == '[') depth++;
                        if ((char)c == ']') depth--;

                        if (!__IsComment((char)c)) pc++;
                    }

                    if (c == EOF) {
                        fprintf(stderr, "ERROR: Couldn't find matching ']' for '[' token.\n");
                        err_flag = true;
                        break;
                    }
                } else {
                    stack_Push(st, pc);
                }
                break;
            case ']':
                if (stack_IsEmpty(st)) {
                    fprintf(stderr, "ERROR: Found ']' token with no matching '['.\n");
                    err_flag = true;
                    break;
                }

                if (memory[mem_ptr] != 0) {
                    //Go back to matching '['
                    uint64_t tgt_pc = stack_Peek(st);
                    while (pc > tgt_pc) {
                        fseek(src, -2L, SEEK_CUR);
                        if (!__IsComment((char)fgetc(src))) pc--;
                    }
                } else {
                    stack_Pop(st);
                }
                break;
            default:
                continue;  //Everything else is dismissed as a comment
        }

        pc++;
    }

    fclose(src);
    stack_Destroy(st);
    st = NULL;
    src = NULL;

    return EXIT_SUCCESS;
}

bool __IsComment(char c) {
    bool is_token = (c == '+' ||
                     c == '-' ||
                     c == '<' ||
                     c == '>' ||
                     c == '.' ||
                     c == ',' ||
                     c == '[' ||
                     c == ']');

    return !is_token;
}