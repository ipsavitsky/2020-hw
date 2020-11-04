#include <stddef.h>

/**
 * \brief structure of a stack
 */
typedef struct {
    void *data;        /**< data of a stack */
    void *stack_top;   /**< pointer to the end of a valid stack */
    size_t stack_size; /**< size of a stack(not always to the last element) */
} Stack;

typedef char Size_elem;

/**
 * initialize a stack
 * \param stack stack to initialize
 * \param data_size maximum size of a stack
 * \return error code
 */
int stack_init(Stack *stack, size_t data_size);

/**
 * finalize a stack
 * \param stack stack to finalize
 */
void stack_finalize(Stack *stack);

/**
 * pop an element to the stack
 * \param stack stack from which to pop
 * \param resp array to which write the result
 * \param size_res size of resp. In case of overflow returns an error
 */
int stack_pop(Stack *stack, void *resp, size_t size_res);

/**
 * push an element to the stack
 * \param stack stack to which to push
 * \param resp array to which write the result
 * \param size_res size of resp. In case of overflow returns an error
 */
int stack_push(Stack *stack, const void *resp, size_t size_res);