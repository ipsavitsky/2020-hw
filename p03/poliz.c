#include "poliz.h"
#include "stack.h"

#define SAFE(call)                  \
    do {                            \
        if ((flag = call) != 0) {   \
            stack_finalize(&stack); \
            return flag;            \
        }                           \
    } while (0)

int compute_RPN(RPN *notation, void *res, size_t res_size) {
    Stack stack;
    int flag;
    if ((flag = stack_init(&stack)) != 0) {
        return flag;
    }
    for (size_t curr_size = 0; curr_size < notation->stack_size;
         curr_size += sizeof(Size_elem) + ((char *)notation->data)[curr_size]) {
        void *elem = &(((char *)notation->data)[curr_size + sizeof(Size_elem)]);
        Calculate_elem func = *((Calculate_elem *)elem);
        SAFE(func(elem, ((char *)notation->data)[curr_size], &stack));
    }
    SAFE(stack_pop(&stack, res, res_size));
    stack_finalize(&stack);
    return 0;
}
