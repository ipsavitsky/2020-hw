#include <stddef.h>

#include "stack.h"

/**
 * struct of an RPN
 */
typedef struct {
    void *data;       /**< input data of rpn */
    size_t data_size; /**< size of data_size */
} RPN;

#define SAFE(call)                  \
    do {                            \
        if ((flag = call) != 0) {   \
            stack_finalize(&stack); \
            return flag;            \
        }                           \
    } while (0)

typedef int (*Calculate_elem)(const void *elem, Size_elem size, Stack *stack);

/**
 * compute the RPN
 * \param notation notation to compute
 * \param res result of the computation
 * \param res_size size of res\
 * \return error code
 */
int RPN_compute(RPN *notation, void *res, size_t res_size);

void RPN_finalize(RPN *notation);