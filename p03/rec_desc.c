#include "rec_desc.h"

#include <stdio.h>
#include <string.h>

#define SAFE(call)                        \
    do {                                  \
        if ((flag = call) != 0) {         \
            RPN_finalize(&conv_notation); \
            return flag;                  \
        }                                 \
    } while (0)

int parse_fact(Expression *expr, RPN *stack_mach, int *cur);
int parse_product(Expression *expr, RPN *stack_mach, int *cur);
int parse_sum(Expression *expr, RPN *stack_mach, int *cur);
int parse_number(Expression *expr, double *res);

int add_elem(const void *elem, Size_elem size, Stack *stack) {
    return stack_push(stack, &(((char *)elem)[sizeof(Calculate_elem)]),
                      size - sizeof(Calculate_elem));
}

int sum_double(const void *elem, Size_elem size, Stack *stack) {
    double elem1, elem2;
    stack_pop(stack, &elem1, sizeof(elem1));
    stack_pop(stack, &elem2, sizeof(elem2));
    double res = elem1 + elem2;
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int mult_double(const void *elem, Size_elem size, Stack *stack) {
    double elem1, elem2;
    stack_pop(stack, &elem1, sizeof(elem1));
    stack_pop(stack, &elem2, sizeof(elem2));
    double res = elem1 * elem2;
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int what1(RPN *expression, int *cur, double elem){
    printf("added %lf to stackmachine to %d\n", elem, *cur);
    Calculate_elem *func_ptr;
    ((char *)expression->data)[*cur] = sizeof(int) + sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression->data)[*cur]);
    *func_ptr = add_elem;
    cur += sizeof(*func_ptr);
    double add_2 = elem;
    memcpy(&(((char *)expression->data)[*cur]), &add_2, sizeof(add_2));
    cur += sizeof(add_2);
    return 0;
}

int what2(RPN *expression, int *cur) {
    printf("added addition function to %d\n", *cur);
    Calculate_elem *func_ptr;
    ((char *)expression->data)[*cur] = sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression->data)[*cur]);
    *func_ptr = sum_double;
    cur += sizeof(Calculate_elem);
    return 0;
}

int init_expression(Expression *expr, char *input) {
    expr->string_form = strdup(input);
    if (expr->string_form == NULL)
        return E_MEM_ALLOC;
    expr->curpointer = expr->string_form;
    return 0;
}

int parse_sum(Expression *expr, RPN *stack_mach, int *cur) {
    parse_product(expr, stack_mach, cur);
    while (*(expr->curpointer) == '+') {
        expr->curpointer++;
        parse_product(expr, stack_mach, cur);
        // what2(stack_mach, cur);
        printf("added addition function to %d\n", *cur);
    }
    return 0;
}

int parse_product(Expression *expr, RPN *stack_mach, int *cur) {
    parse_fact(expr, stack_mach, cur);
    while (*(expr->curpointer) == '*') {
        expr->curpointer++;
        parse_fact(expr, stack_mach, cur);
        // what2(stack_mach, cur);
        printf("added multiplication function to %d\n", *cur);
    }
    return 0;
}

int parse_fact(Expression *expr, RPN *stack_mach, int *cur) {
    double ret;
    if ((*(expr->curpointer) > '0') && (*(expr->curpointer) < '9')){
        parse_number(expr, &ret);
        // what1(stack_mach, cur, ret);
        printf("added %f to stackmachine to %d\n", ret, *cur);
        return 0;
    }
    else if (*(expr->curpointer) == '(') {
        expr->curpointer++;
        parse_sum(expr, stack_mach, cur);
        // TODO: check for )
        expr->curpointer++;
        return 0;
    } else
        return E_UNEXPECTED_SYMBOL;
}

int parse_number(Expression *expr, double *res) {
    double ret = 0;
    while ((*(expr->curpointer) > '0') && (*(expr->curpointer) < '9')) {
        ret *= 10;
        ret += *(expr->curpointer) - '0';
        expr->curpointer++;
    }
    if (*(expr->curpointer) == '.') {
        expr->curpointer++;
        double weight = 1;
        while ((*(expr->curpointer) > '0') && (*(expr->curpointer) < '9')) {
            weight /= 10;
            ret += (*(expr->curpointer) - '0') * weight;
            expr->curpointer++;
        }
    }
    *res = ret;
    return 0;
}

int compute_expression(Expression *expr, double *res) {
    RPN stack_machine;
    RPN_init(&stack_machine, 100);
    double ret = 0;
    int cur = 0;
    parse_sum(expr, &stack_machine, &cur);
    if (*(expr->curpointer) == '\0'){
        // RPN_compute(&stack_machine, &ret, 100);
        *res = ret;
        RPN_finalize(&stack_machine);
        return 0;
    }
    printf("something went wrong\n");
    return E_UNEXPECTED_SYMBOL;
}