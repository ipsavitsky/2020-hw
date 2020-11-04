#include "stack.h"

#include <stdlib.h>
#include <string.h>

// do I really need this?
#define MEM_SAFE(call)              \
    do {                            \
        if (call == NULL) {         \
            stack_finalize(&stack); \
            return 1;               \
        }                           \
    } while (0)

// TODO: ERROR CODES!

int stack_init(Stack *stack, size_t data_size) {
    // TODO: replace char with smth
    stack->data = calloc(data_size, sizeof(char));
    stack->stack_top = stack->data;
    stack->stack_size = 0;
    return 0;
}

void stack_finalize(Stack *stack) { free(stack->data); }

int stack_pop(Stack *stack, void *resp, size_t size_res) {
    void *cur = stack->stack_top;
    cur = (char *)cur - sizeof(Size_elem);
    size_t size = *((char *)cur);
    cur = (char *)cur - size;
    memcpy(resp, cur, size);
    stack->stack_top = cur;
    return 0;
}

int stack_push(Stack *stack, void *resp, size_t size_res) {
    memcpy((char *)stack->stack_top, resp, size_res);
    memcpy((char *)stack->stack_top + size_res, &size_res, sizeof(Size_elem));
    stack->stack_top = &(((char *)stack->stack_top)[size_res + sizeof(Size_elem)]);
    stack->stack_size += size_res;
    return 0;
}