#include <stddef.h>
#include "stack.h"

typedef struct{
    void* data;
    size_t data_size;
} RPN;

#define SAFE(call)                  \
    do {                            \
        if ((flag = call) != 0) {   \
            stack_finalize(&stack); \
            return flag;            \
        }                           \
    } while (0)

typedef int (*Calculate_elem)(const void *elem, Size_elem size, Stack *stack);

int RPN_compute(RPN *notation, void *res, size_t res_size);