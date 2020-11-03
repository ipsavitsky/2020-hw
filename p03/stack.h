typedef struct Stack Stack;

int stack_init(Stack *stack);
void stack_finalize(Stack *stack);
int stack_pop(Stack *stack, void *resp, size_t size_res);