#include "stack.h"

/**
 * struct of an RPN
 */
typedef struct {
    void *data;       ///< input data of rpn
    size_t data_size; ///< size of data_size
    size_t occupied;  ///< tracking of the last written element
} RPN;

typedef int (*Calculate_elem)(const void *elem, Size_elem size, Stack *stack);

/**
 * compute the RPN
 * \param notation notation to compute
 * \param res result of the computation
 * \param res_size size of res
 * \return error code
 */
int RPN_compute(RPN *notation, void *res, size_t res_size);


/**
 * initialize an RPN
 * \param notation RPN to initialize
 * \param size size of the notation
 * \exception E_MEM_ALLOC Thrown in case of memory allocation error
 * \return error code
 */
int RPN_init(RPN *notation, size_t size);


/**
 * finalize an RPN
 * \param notation RPN to finalize
 */
void RPN_finalize(RPN *notation);