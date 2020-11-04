#include "RPN.h"

#include <stdlib.h>

#define SAFE(call)                  \
    do {                            \
        if ((flag = call) != 0) {   \
            stack_finalize(&stack); \
            return flag;            \
        }                           \
    } while (0)

int RPN_compute(RPN *notation, void *res, size_t res_size) {
    Stack stack;
    int flag;
    if ((flag = stack_init(&stack, notation->data_size)) != 0) return flag;
    for (size_t cur_size = 0; cur_size < notation->data_size;
         cur_size += sizeof(Size_elem) + ((char *)notation->data)[cur_size]) {
        void *elem = &(((char *)notation->data)[cur_size + sizeof(Size_elem)]);
        Calculate_elem func = *((Calculate_elem *)elem);
        SAFE(func(elem, ((char *)notation->data)[cur_size], &stack));
    }
    SAFE(stack_pop(&stack, res, res_size));
    stack_finalize(&stack);
    return 0;
}

int RPN_init(RPN *notation, size_t size) {
    if ((notation->data = malloc(size)) == NULL) return E_MEM_ALLOC;
    return 0;
}

void RPN_finalize(RPN *notation) { free(notation->data); }
