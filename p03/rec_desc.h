#include "RPN.h"

typedef struct{
    char *string_form;
    char *curpointer;
} Expression;

int compute_expression(Expression *expr, double *res);

int init_expression(Expression *expr, char *input);

void finalize_expression(Expression *expr);