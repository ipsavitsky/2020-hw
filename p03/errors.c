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
        case E_UNBALANCED_BRACKET:
            fprintf(stderr, "Error: unbalanced bracket\n");
            break;
        default:
            fprintf(stderr, "Error: unknown(%d)", err);
    }
}