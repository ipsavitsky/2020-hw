#include "RPN.h"

typedef struct{
    char *string_form;
    char *curpointer;
    char *recursive_pointer;
} Expression;

int compute_expression(Expression *expr, double *res);

int init_expression(Expression *expr, char *input);

void finalize_expression(Expression *expr);