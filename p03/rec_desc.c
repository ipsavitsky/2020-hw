#include "rec_desc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE(call)                        \
    do {                                  \
        if ((flag = call) != 0) {         \
            RPN_finalize(&conv_notation); \
            return flag;                  \
        }                                 \
    } while (0)

int parse_fact(Expression *expr, RPN *stack_mach);
int parse_product(Expression *expr, RPN *stack_mach);
int parse_sum(Expression *expr, RPN *stack_mach);
int parse_literal(Expression *expr, RPN *stack_mach);

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

int put_number_in_RPN(RPN *expression, double elem) {
    // printf("added %lf to stackmachine to %d\n", elem, *cur);
    Calculate_elem *func_ptr;
    ((char *)expression->data)[expression->occupied] =
        sizeof(double) + sizeof(Calculate_elem);
    expression->occupied += sizeof(Size_elem);
    func_ptr =
        (Calculate_elem *)&(((char *)expression->data)[expression->occupied]);
    *func_ptr = add_elem;
    expression->occupied += sizeof(*func_ptr);
    double el = elem;
    memcpy(&(((char *)expression->data)[expression->occupied]), &el,
           sizeof(el));
    expression->occupied += sizeof(el);
    return 0;
}

int put_addition_in_RPN(RPN *expression) {
    // printf("added addition function to %d\n", *cur);
    Calculate_elem *func_ptr;
    ((char *)expression->data)[expression->occupied] = sizeof(Calculate_elem);
    expression->occupied += sizeof(Size_elem);
    func_ptr =
        (Calculate_elem *)&(((char *)expression->data)[expression->occupied]);
    *func_ptr = sum_double;
    expression->occupied += sizeof(Calculate_elem);
    return 0;
}

int put_multiplication_in_RPN(RPN *expression) {
    // printf("added addition function to %d\n", *cur);
    Calculate_elem *func_ptr;
    ((char *)expression->data)[expression->occupied] = sizeof(Calculate_elem);
    expression->occupied += sizeof(Size_elem);
    func_ptr =
        (Calculate_elem *)&(((char *)expression->data)[expression->occupied]);
    *func_ptr = mult_double;
    expression->occupied += sizeof(Calculate_elem);
    return 0;
}

int init_expression(Expression *expr, char *input) {
    expr->string_form = strdup(input);
    if (expr->string_form == NULL) return E_MEM_ALLOC;
    expr->curpointer = expr->string_form;
    return 0;
}

int parse_sum(Expression *expr, RPN *stack_mach) {
    parse_product(expr, stack_mach);
    while (*(expr->curpointer) == '+') {
        expr->curpointer++;
        parse_product(expr, stack_mach);
        put_addition_in_RPN(stack_mach);
        // printf("added addition function to %ld\n", stack_mach->occupied);
    }
    return 0;
}

int parse_product(Expression *expr, RPN *stack_mach) {
    parse_fact(expr, stack_mach);
    while (*(expr->curpointer) == '*') {
        expr->curpointer++;
        parse_fact(expr, stack_mach);
        put_multiplication_in_RPN(stack_mach);
        // printf("added multiplication function to %ld\n",
        // stack_mach->occupied);
    }
    return 0;
}

int parse_fact(Expression *expr, RPN *stack_mach) {
    if ((*(expr->curpointer) > '0') && (*(expr->curpointer) < '9')) {
        parse_literal(expr, stack_mach);
        // printf("added %f to stackmachine to %ld\n", ret,
        // stack_mach->occupied);
        return 0;
    } else if (*(expr->curpointer) == '(') {
        expr->curpointer++;
        parse_sum(expr, stack_mach);
        // TODO: check for )
        expr->curpointer++;
        return 0;
    } else
        return E_UNEXPECTED_SYMBOL;
}

int parse_literal(Expression *expr, RPN *stack_mach) {
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
    put_number_in_RPN(stack_mach, ret);
    return 0;
}

int compute_expression(Expression *expr, double *res) {
    RPN stack_machine;
    RPN_init(&stack_machine, 1000);
    double ret = 0;
    parse_sum(expr, &stack_machine);

    stack_machine.data = realloc(stack_machine.data, stack_machine.occupied);
    stack_machine.data_size = stack_machine.occupied;

    if (*(expr->curpointer) == '\0') {
        RPN_compute(&stack_machine, &ret, sizeof(double));
        // printf("err = %d\n", err);
        // printf("RPN COMPUTED: %lf\n", ret);
        *res = ret;
        RPN_finalize(&stack_machine);
        return 0;
    }
    printf("something went wrong\n");
    return E_UNEXPECTED_SYMBOL;
}

void finalize_expression(Expression *expr) { free(expr->string_form); }