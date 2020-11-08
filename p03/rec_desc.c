#include "rec_desc.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE(call)                                \
    do {                                          \
        if ((flag = call) != 0) return flag; \
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
    int flag;
    SAFE(stack_pop(stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(stack, &elem1, sizeof(elem2)));
    double res = elem1 + elem2;
    printf("%lf + %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    SAFE(stack_push(stack, &res, sizeof(res)));
    return 0;
}

int sub_double(const void *elem, Size_elem size, Stack *stack) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(stack, &elem1, sizeof(elem2)));
    double res = elem1 - elem2;
    printf("%lf - %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    SAFE(stack_push(stack, &res, sizeof(res)));
    return 0;
}

int mult_double(const void *elem, Size_elem size, Stack *stack) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(stack, &elem1, sizeof(elem2)));
    double res = elem1 * elem2;
    printf("%lf * %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    SAFE(stack_push(stack, &res, sizeof(res)));
    return 0;
}

int div_double(const void *elem, Size_elem size, Stack *stack) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(stack, &elem1, sizeof(elem2)));
    double eps = 0.0001;
    if (fabs(elem2) < eps) return E_ZERO_DIVISION;
    double res = elem1 / elem2;
    printf("%lf / %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    SAFE(stack_push(stack, &res, sizeof(res)));
    return 0;
}

int neg(const void *elem, Size_elem size, Stack *stack) {
    double num;
    int flag;
    SAFE(stack_pop(stack, &num, sizeof(num)));
    num = -num;
    SAFE(stack_push(stack, &num, sizeof(num)));
    return 0;
}

int lookup_var(const void *elem, Size_elem size, Stack *stack) {
    // printf("trying to lookup: %s\n", &(((char
    // *)elem)[sizeof(Calculate_elem)]));
    void *elem_ptr = &(((char *)elem)[sizeof(Calculate_elem)]);
    size_t name_size = size - sizeof(Calculate_elem);
    if (strncmp(elem_ptr, "avds", name_size) == 0) {
        double p = 0;
        printf("adding variable avds = %lf\n", p);
        return stack_push(stack, &p, sizeof(p));
    } else if (strncmp(elem_ptr, "e", name_size) == 0) {
        double p = 2.71;
        printf("adding variable e = %lf\n", p);
        return stack_push(stack, &p, sizeof(p));
    } else if (strncmp(elem_ptr, "wow", name_size) == 0) {
        double p = 2.05;
        printf("adding variable wow = %lf\n", p);
        return stack_push(stack, &p, sizeof(p));
    } else if (strncmp(elem_ptr, "abcdef", name_size) == 0) {
        double p = 29;
        printf("adding variable abcdef = %lf\n", p);
        return stack_push(stack, &p, sizeof(p));
    } else {
        printf("lookup failed\n");
        return E_UNKNOWN_VAR;
    }
    return 0;
}



int put_var_in_RPN(RPN *expression, char *name, int name_size) {
    if (expression->occupied + sizeof(Size_elem) + sizeof(Calculate_elem) +
            name_size >=
        expression->data_size) {
        return E_OVERFLOW;
    }
    Calculate_elem *func_ptr;
    ((char *)expression->data)[expression->occupied] =
        name_size + sizeof(Calculate_elem);
    expression->occupied += sizeof(Size_elem);
    func_ptr =
        (Calculate_elem *)&(((char *)expression->data)[expression->occupied]);
    *func_ptr = lookup_var;
    expression->occupied += sizeof(*func_ptr);
    memcpy(&(((char *)expression->data)[expression->occupied]), name,
           name_size);
    expression->occupied += name_size;
    return 0;
}

int put_number_in_RPN(RPN *expression, double elem) {
    if (expression->occupied + sizeof(Size_elem) + sizeof(Calculate_elem) +
            sizeof(double) >=
        expression->data_size) {
        return E_OVERFLOW;
    }
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
    if (expression->occupied + sizeof(Size_elem) + sizeof(Calculate_elem) >=
        expression->data_size) {
        return E_OVERFLOW;
    }
    // printf("added function to %ld\n", expression->occupied);
    Calculate_elem *func_ptr;
    ((char *)expression->data)[expression->occupied] = sizeof(Calculate_elem);
    expression->occupied += sizeof(Size_elem);
    func_ptr =
        (Calculate_elem *)&(((char *)expression->data)[expression->occupied]);
    *func_ptr = func_in;
    expression->occupied += sizeof(*func_ptr);
    return 0;
}



int parse_sum(Expression *expr, RPN *stack_mach) {
    int flag;
    SAFE(parse_product(expr, stack_mach));
    char operation;
    // printf("parse_sum = %c\n", *(expr->curpointer));
    while ((*(expr->curpointer) == '+') || (*(expr->curpointer) == '-')) {
        operation = *(expr->curpointer);
        expr->curpointer++;
        SAFE(parse_product(expr, stack_mach));
        if (operation == '+')
            SAFE(put_func_in_RPN(stack_mach, sum_double));
        else
            SAFE(put_func_in_RPN(stack_mach, sub_double));
    }
    return 0;
}

int parse_product(Expression *expr, RPN *stack_mach) {
    int flag;
    SAFE(parse_fact(expr, stack_mach));
    char operation;
    while ((*(expr->curpointer) == '*') || (*(expr->curpointer) == '/')) {
        operation = *(expr->curpointer);
        expr->curpointer++;
        SAFE(parse_fact(expr, stack_mach));
        if (operation == '*')
            SAFE(put_func_in_RPN(stack_mach, mult_double));
        else
            SAFE(put_func_in_RPN(stack_mach, div_double));
    }
    return 0;
}

int parse_fact(Expression *expr, RPN *stack_mach) {
    int flag;
    short neg_flag = 1;
    if (*(expr->curpointer) == '-' &&
        ((expr->curpointer == expr->string_form) ||
         (*(expr->curpointer - 1) == '('))) {
        neg_flag = -1;
        expr->curpointer++;
    }
    if (((*(expr->curpointer) >= '0') && (*(expr->curpointer) <= '9'))) {
        SAFE(parse_literal(expr, stack_mach));
        // printf("added %f to stackmachine to %ld\n", ret,
        // stack_mach->occupied);
    } else if (*(expr->curpointer) == '(') {
        expr->curpointer++;
        SAFE(parse_sum(expr, stack_mach));
        if (*(expr->curpointer) != ')') return E_UNBALANCED_BRACKET;
        expr->curpointer++;
    } else if (isalpha(*(expr->curpointer))) {
        SAFE(parse_variable(expr, stack_mach));
    } else
        return E_UNEXPECTED_SYMBOL;
    if (neg_flag == -1) SAFE(put_func_in_RPN(stack_mach, neg));
    return 0;
}

int parse_variable(Expression *expr, RPN *stack_mach) {
    int flag;
    int size = 0;
    unsigned char var[6];
    while (isalnum(*(expr->curpointer)) && (size < 6)) {
        var[size++] = *(expr->curpointer);
        expr->curpointer++;
    }
    // printf("variable found: %s(%d)\n", var, size);
    SAFE(put_var_in_RPN(stack_mach, var, size));
    return 0;
}

int parse_literal(Expression *expr, RPN *stack_mach) {
    double ret = 0;
    int flag;

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
    SAFE(put_number_in_RPN(stack_mach, ret));
    return 0;
}



int compute_expression(Expression *expr, double *res) {
    RPN stack_machine;
    int flag;
    size_t rpn_estimate = strlen(expr->string_form) * (sizeof(Size_elem) + sizeof(Calculate_elem) + sizeof(double));
    SAFE(RPN_init(&stack_machine, rpn_estimate));
    double ret = 0;
    if((flag = parse_sum(expr, &stack_machine)) != 0){
        RPN_finalize(&stack_machine);
        return flag;
    }

    stack_machine.data = realloc(stack_machine.data, stack_machine.occupied);
    stack_machine.data_size = stack_machine.occupied;

    if (*(expr->curpointer) == '\0') {
        if((flag = RPN_compute(&stack_machine, &ret, sizeof(double))) != 0){
            RPN_finalize(&stack_machine);
            return flag;
        }
        // printf("err = %d\n", err);
        // printf("RPN COMPUTED: %lf\n", ret);
        *res = ret;
        RPN_finalize(&stack_machine);
        return 0;
    }
    RPN_finalize(&stack_machine);
    return E_UNEXPECTED_SYMBOL;
}

int init_expression(Expression *expr, char *input) {
    expr->string_form = strdup(input);
    if (expr->string_form == NULL) return E_MEM_ALLOC;
    expr->curpointer = expr->string_form;
    return 0;
}

void finalize_expression(Expression *expr) { free(expr->string_form); }