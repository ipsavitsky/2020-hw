#include "RPN.h"


/**
 * structure of an infex expression
 */
typedef struct{
    unsigned char *string_form; ///< string with the expression
    unsigned char *curpointer; ///< pointer to the current symbol(used internally)
} Expression;


/**
 * compute an expression
 * \param expr expression to compute
 * \param res result of the computation
 * \exception E_UNEXPECTED_SYMBOL Thrown in case of error in parsing a string 
 * \return error code
 */
int compute_expression(Expression *expr, double *res);


/**
 * initialize an expression
 * \param expr which expression to initialize
 * \param input string with which to initialize the expression
 * \exception E_MEM_ALLOC Thrown in case of memory allocation error
 * \return error code
 */
int init_expression(Expression *expr, char *input);


/**
 * finalize an expression
 * \param expr expression to finalize
 */
void finalize_expression(Expression *expr);