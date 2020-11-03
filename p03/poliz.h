#include <stddef.h>
#include <stack.h>

typedef struct{
    void* data;
    size_t stack_size;
} RPN;

typedef char Size_elem;

typedef int (*Calculate_elem)(const void *elem, Size_elem size, Stack *stack);

int compute_RPN(RPN *notation, void *res, size_t res_size);