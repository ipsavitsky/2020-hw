#include "errors.h"
#include <stdio.h>

void err_print(int err){
    switch (err)
    {
        case E_MEM_ALLOC:
            fprintf(stderr, "Error: memory allocation\n");
            break;
        case E_OVERFLOW:
            fprintf(stderr, "Error: dynamic structure overflow\n");
            break;
        case E_UNDERFLOW:
            fprintf(stderr, "Error: dynamic structure underflow\n");
            break;
        case E_UNEXPECTED_SYMBOL:
            fprintf(stderr, "Error: unexpected symbol\n");
            break;
        case E_ZERO_DIVISION:
            fprintf(stderr, "Error: zero division prohibited\n");
            break;
        case E_UNKNOWN_VAR:
            fprintf(stderr, "Error: unknown variable\n");
            break;
        case E_UNBALANCED_LB:
            fprintf(stderr, "Error: unbalanced \'(\'\n");
            break;
        case E_UNBALANCED_RB:
            fprintf(stderr, "Error: unbalanced \')\'\n");
            break;
        case E_MISSED_OPERATOR:
            fprintf(stderr, "Error: missed operator\n");
            break;
        default:
            fprintf(stderr, "Error: unknown(%d)\n", err);
            break;
    }
}