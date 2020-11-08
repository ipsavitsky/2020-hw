#include "rec_desc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

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
int parse_variable(Expression *expr, RPN *stack_mach);

int add_elem(const void *elem, Size_elem size, Stack *stack) {
    return stack_push(stack, &(((char *)elem)[sizeof(Calculate_elem)]),
                      size - sizeof(Calculate_elem));
}

int sum_double(const void *elem, Size_elem size, Stack *stack) {
    double elem1, elem2;
    stack_pop(stack, &elem2, sizeof(elem1));
    stack_pop(stack, &elem1, sizeof(elem2));
    double res = elem1 + elem2;
    printf("%lf + %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int sub_double(const void *elem, Size_elem size, Stack *stack) {
    double elem1, elem2;
    stack_pop(stack, &elem2, sizeof(elem1));
    stack_pop(stack, &elem1, sizeof(elem2));
    double res = elem1 - elem2;
    printf("%lf - %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int mult_double(const void *elem, Size_elem size, Stack *stack) {
    double elem1, elem2;
    stack_pop(stack, &elem2, sizeof(elem1));
    stack_pop(stack, &elem1, sizeof(elem2));
    double res = elem1 * elem2;
    printf("%lf * %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int div_double(const void *elem, Size_elem size, Stack *stack) {
    double elem1, elem2;
    stack_pop(stack, &elem2, sizeof(elem1));
    stack_pop(stack, &elem1, sizeof(elem2));
    double eps = 0.0001;
    if (fabs(elem2) < eps) return E_ZERO_DIVISION;
    double res = elem1 / elem2;
    printf("%lf / %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int neg(const void *elem, Size_elem size, Stack *stack) {
    double num;
    stack_pop(stack, &num, sizeof(num));
    num = -num;
    stack_push(stack, &num, sizeof(num));
    return 0;
}

int lookup_var(const void *elem, Size_elem size, Stack *stack) {
    // printf("trying to lookup: %s\n", &(((char *)elem)[sizeof(Calculate_elem)]));
    void *elem_ptr = &(((char *)elem)[sizeof(Calculate_elem)]);
    size_t name_size =  size - sizeof(Calculate_elem);
    if (strncmp(elem_ptr, "avds", name_size) == 0) {
        double p = 0;
        printf("adding variable avds = %lf\n", p);
        return stack_push(stack, &p, sizeof(p));
    }
    else if(strncmp(elem_ptr, "e", name_size) == 0){
        double p = 2.71;
        printf("adding variable e = %lf\n", p);
        return stack_push(stack, &p, sizeof(p));
    }
    else{
        printf("lookup failed\n");
    }
    return 0;
}

int put_var_in_RPN(RPN *expression, char *name, int name_size){
    printf("added variable %s to stackmachine to %ld\n", name, expression->occupied);
    Calculate_elem *func_ptr;
    ((char *)expression->data)[expression->occupied] = name_size + sizeof(Calculate_elem);
    expression->occupied += sizeof(Size_elem);
    func_ptr =
        (Calculate_elem *)&(((char *)expression->data)[expression->occupied]);
    *func_ptr = lookup_var;
    expression->occupied += sizeof(*func_ptr);
    memcpy(&(((char *)expression->data)[expression->occupied]), name, name_size);
    expression->occupied += name_size;
    return 0;
}

int put_number_in_RPN(RPN *expression, double elem) {
    printf("added %lf to stackmachine to %ld\n", elem, expression->occupied);
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

int put_func_in_RPN(RPN *expression, Calculate_elem func_in) {
    printf("added function to %ld\n", expression->occupied);
    Calculate_elem *func_ptr;
    ((char *)expression->data)[expression->occupied] = sizeof(Calculate_elem);
    expression->occupied += sizeof(Size_elem);
    func_ptr =
        (Calculate_elem *)&(((char *)expression->data)[expression->occupied]);
    *func_ptr = func_in;
    expression->occupied += sizeof(*func_ptr);
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
    char operation;
    // printf("parse_sum = %c\n", *(expr->curpointer));
    while ((*(expr->curpointer) == '+') || (*(expr->curpointer) == '-')) {
        operation = *(expr->curpointer);
        expr->curpointer++;
        parse_product(expr, stack_mach);
        if (operation == '+')
            put_func_in_RPN(stack_mach, sum_double);
        else
            put_func_in_RPN(stack_mach, sub_double);
    }
    return 0;
}

int parse_product(Expression *expr, RPN *stack_mach) {
    parse_fact(expr, stack_mach);
    char operation;
    while ((*(expr->curpointer) == '*') || (*(expr->curpointer) == '/')) {
        operation = *(expr->curpointer);
        expr->curpointer++;
        parse_fact(expr, stack_mach);
        if (operation == '*')
            put_func_in_RPN(stack_mach, mult_double);
        else
            put_func_in_RPN(stack_mach, div_double);
    }
    return 0;
}

int parse_fact(Expression *expr, RPN *stack_mach) {
    short neg_flag = 1;
    if (*(expr->curpointer) == '-' && ((expr->curpointer == expr->string_form) || (*(expr->curpointer - 1) == '('))) {
        neg_flag = -1;
        expr->curpointer++;
    }
    if (((*(expr->curpointer) >= '0') && (*(expr->curpointer) <= '9'))) {
        parse_literal(expr, stack_mach);
        // printf("added %f to stackmachine to %ld\n", ret,
        // stack_mach->occupied);
    } else if (*(expr->curpointer) == '(') {
        expr->curpointer++;
        parse_sum(expr, stack_mach);
        // TODO: check for )
        expr->curpointer++;
    } else if(isalpha(*(expr->curpointer))){
        parse_variable(expr, stack_mach);
    } else
        return E_UNEXPECTED_SYMBOL;
    if (neg_flag == -1) put_func_in_RPN(stack_mach, neg);
    return 0;
}

int parse_variable(Expression *expr, RPN *stack_mach){
    int size = 0;
    unsigned char var[6];
    while (isalpha(*(expr->curpointer)) && (size < 6)){
        var[size++] = *(expr->curpointer);
        expr->curpointer++;
    }
    // size++;
    // var[size] = '\0';
    printf("variable found: %s(%d)\n", var, size);
    put_var_in_RPN(stack_mach, var, size);
    return 0;
}

int parse_literal(Expression *expr, RPN *stack_mach) {
    double ret = 0;
    
    while ((*(expr->curpointer) >= '0') && (*(expr->curpointer) <= '9')) {
        ret *= 10;
        ret += *(expr->curpointer) - '0';
        expr->curpointer++;
    }
    if (*(expr->curpointer) == '.') {
        expr->curpointer++;
        double weight = 1;
        while ((*(expr->curpointer) >= '0') && (*(expr->curpointer) <= '9')) {
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
    RPN_finalize(&stack_machine);
    printf("something went wrong on %c\n", *(expr->curpointer));
    return E_UNEXPECTED_SYMBOL;
}

void finalize_expression(Expression *expr) { free(expr->string_form); }