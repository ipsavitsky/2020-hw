#include "errors.h"
#include <stdio.h>

void err_print(int err){
    switch (err)
    {
        case E_MEM_ALLOC:
            printf("Error:memory allocation\n");
            break;
        case E_OVERFLOW:
            printf("Error: dynamic structure overflow\n");
            break;
        case E_UNDERFLOW:
            printf("Error: dynamic structure underflow\n");
            break;
        case E_UNEXPECTED_SYMBOL:
            printf("Error: unexpected symbol\n");
            break;
        case E_ZERO_DIVISION:
            printf("Error: zero division prohibited\n");
            break;
        case E_UNKNOWN_VAR:
            printf("Error: unknown variable\n");
            break;
        case E_UNBALANCED_BRACKET:
            printf("Error: unbalanced bracket\n");
            break;
        default:
            printf("Error: unknown(%d)", err);
    }
}