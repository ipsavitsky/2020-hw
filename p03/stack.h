#include <stddef.h>
typedef struct {
    void *data;
    void *stack_top;
    size_t stack_size;
} Stack;

typedef char Size_elem;

int stack_init(Stack *stack, size_t data_size);
void stack_finalize(Stack *stack);
int stack_pop(Stack *stack, void *resp, size_t size_res);
int stack_push(Stack *stack, void *resp, size_t size_res);