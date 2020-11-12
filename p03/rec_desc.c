#include "rec_desc.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE(call)                           \
    do {                                     \
        if ((flag = call) != 0) return flag; \
    } while (0)

int parse_fact(Expression *expr, RPN *stack_mach);
int parse_product(Expression *expr, RPN *stack_mach);
int parse_sum(Expression *expr, RPN *stack_mach);
int parse_literal(Expression *expr, RPN *stack_mach);
int parse_variable(Expression *expr, RPN *stack_mach);

int add_elem(Calculation_data *data) {
    return stack_push(data->stack,
                      &(((char *)(data->elem))[sizeof(Calculate_elem)]),
                      data->size - sizeof(Calculate_elem));
}

int sum_double(Calculation_data *data) {
    double elem1, elem2;
    int flag;
    assert(data->stack != NULL);
    SAFE(stack_pop(data->stack, &elem2, sizeof(elem2)));
    SAFE(stack_pop(data->stack, &elem1, sizeof(elem1)));
    double res = elem1 + elem2;
    printf("%lf + %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    SAFE(stack_push(data->stack, &res, sizeof(res)));
    return 0;
}

int sub_double(Calculation_data *data) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(data->stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(data->stack, &elem1, sizeof(elem2)));
    double res = elem1 - elem2;
    printf("%lf - %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    SAFE(stack_push(data->stack, &res, sizeof(res)));
    return 0;
}

int mult_double(Calculation_data *data) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(data->stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(data->stack, &elem1, sizeof(elem2)));
    double res = elem1 * elem2;
    printf("%lf * %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    SAFE(stack_push(data->stack, &res, sizeof(res)));
    return 0;
}

int div_double(Calculation_data *data) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(data->stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(data->stack, &elem1, sizeof(elem2)));
    double eps = 0.0001;
    if (fabs(elem2) < eps) return E_ZERO_DIVISION;
    double res = elem1 / elem2;
    printf("%lf / %lf = %lf\n", elem1, elem2, res);
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    SAFE(stack_push(data->stack, &res, sizeof(res)));
    return 0;
}

int neg(Calculation_data *data) {
    double num;
    int flag;
    SAFE(stack_pop(data->stack, &num, sizeof(num)));
    num = -num;
    SAFE(stack_push(data->stack, &num, sizeof(num)));
    return 0;
}

int lookup_var(Calculation_data *data) {
    void *elem_ptr = &(((char *)(data->elem))[sizeof(Calculate_elem)]);
    size_t lookup_name_size = data->size - sizeof(Calculate_elem);

    for (int i = 0; i < data->v_tab->var_num; ++i) {
        // TODO: redo with strlen()
        size_t name_size = strlen(data->v_tab->vars[i].name);
        if (name_size != lookup_name_size) continue;
        if (strncmp(elem_ptr, data->v_tab->vars[i].name, name_size) == 0) {
            printf("%s = %lf\n", data->v_tab->vars[i].name,
                   *((double *)(data->v_tab->vars[i].data)));
            return stack_push(data->stack, data->v_tab->vars[i].data,
                              sizeof(double));
        }
    }
    // printf("lookup failed");
    return E_UNKNOWN_VAR;
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
    expression->occupied += sizeof(Calculate_elem);
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
    expression->occupied += sizeof(Calculate_elem);
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
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    while ((*(expr->curpointer) == '+') || (*(expr->curpointer) == '-')) {
        operation = *(expr->curpointer++);
        SAFE(parse_product(expr, stack_mach));
        switch (operation) {
            case '+':
                SAFE(put_func_in_RPN(stack_mach, sum_double));
                break;
            case '-':
                SAFE(put_func_in_RPN(stack_mach, sub_double));
                break;
            default:
                break;
        }
    }
    return 0;
}

int parse_product(Expression *expr, RPN *stack_mach) {
    int flag;
    SAFE(parse_fact(expr, stack_mach));
    char operation;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    while ((*(expr->curpointer) == '*') || (*(expr->curpointer) == '/')) {
        operation = *(expr->curpointer++);
        SAFE(parse_fact(expr, stack_mach));
        switch (operation) {
            case '*':
                SAFE(put_func_in_RPN(stack_mach, mult_double));
                break;
            case '/':
                SAFE(put_func_in_RPN(stack_mach, div_double));
                break;
            default:
                break;
        }
    }
    return 0;
}

int parse_fact(Expression *expr, RPN *stack_mach) {
    int flag;
    short neg_flag = 1;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    if (*(expr->curpointer) == '-' &&
        ((expr->curpointer == expr->string_form) ||
         (*(expr->curpointer - 1) == '('))) {
        neg_flag = -1;
        ++(expr->curpointer);
    }
    if (isdigit(*(expr->curpointer)) || (*(expr->curpointer) == '.')) {
        SAFE(parse_literal(expr, stack_mach));
    } else if (*(expr->curpointer) == '(') {
        ++(expr->curpointer);
        SAFE(parse_sum(expr, stack_mach));
        // FIXME: this should be here, but everything works without it
        while (isspace(*(expr->curpointer))) ++(expr->curpointer);
        if (*(expr->curpointer) != ')') return E_UNBALANCED_LB;
        ++(expr->curpointer);
        // FIXME: i have no idea why this is here but without it everything
        // breaks
        while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    } else if (isalpha(*(expr->curpointer))) {
        SAFE(parse_variable(expr, stack_mach));
    } else {
        printf("un_s = %c\n", *(expr->curpointer));
        return E_UNEXPECTED_SYMBOL;
    }
    if (neg_flag == -1) SAFE(put_func_in_RPN(stack_mach, neg));
    return 0;
}

int parse_variable(Expression *expr, RPN *stack_mach) {
    int flag;
    int size = 0;
    unsigned char var[6];
    while (isalnum(*(expr->curpointer)) && (size < 6))
        var[size++] = *(expr->curpointer++);
    // printf("variable found: %s(  %d)\n", var, size);
    SAFE(put_var_in_RPN(stack_mach, var, size));
    return 0;
}

int add_variable_to_table(Expression *expr, const char *name, double num) {
    ++(expr->v_tab->var_num);
    expr->v_tab->vars =
        realloc(expr->v_tab->vars, expr->v_tab->var_num * sizeof(Var_data));
    if (expr->v_tab->vars == NULL) return E_MEM_ALLOC;
    expr->v_tab->vars[expr->v_tab->var_num - 1].data_size = sizeof(double);
    expr->v_tab->vars[expr->v_tab->var_num - 1].data = malloc(sizeof(double));
    if (expr->v_tab->vars[expr->v_tab->var_num - 1].data == NULL)
        return E_MEM_ALLOC;
    memcpy(expr->v_tab->vars[expr->v_tab->var_num - 1].data, &num,
           sizeof(double));
    strcpy(expr->v_tab->vars[expr->v_tab->var_num - 1].name, name);
    return 0;
}

int parse_literal(Expression *expr, RPN *stack_mach) {
    double ret = 0;
    int flag;
    // expr->curpointer = strtok(expr->curpointer, " +-*/()");
    char *proxy_ptr = NULL;
    ret = strtod(expr->curpointer, &proxy_ptr);
    // printf("%lf\n", ret);
    expr->curpointer = proxy_ptr;
    SAFE(put_number_in_RPN(stack_mach, ret));
    return 0;
}

int compute_expression(Expression *expr, double *res) {
    RPN stack_machine;
    int flag;
    size_t rpn_estimate =
        strlen(expr->string_form) *
        (sizeof(Size_elem) + sizeof(Calculate_elem) + sizeof(double));
    SAFE(RPN_init(&stack_machine, rpn_estimate));
    double ret = 0;

    if ((flag = parse_sum(expr, &stack_machine)) != 0) {
        RPN_finalize(&stack_machine);
        return flag;
    }

    stack_machine.data = realloc(stack_machine.data, stack_machine.occupied);
    stack_machine.data_size = stack_machine.occupied;

    if (*(expr->curpointer) == '\0') {
        if ((flag = RPN_compute(&stack_machine, &ret, sizeof(double),
                                expr->v_tab)) != 0) {
            RPN_finalize(&stack_machine);
            return flag;
        }
        *res = ret;
        RPN_finalize(&stack_machine);
        return 0;
    } else if (*(expr->curpointer) == ')') {
        RPN_finalize(&stack_machine);
        return E_UNBALANCED_RB;
    } else if (isalnum(*(expr->curpointer))) {
        RPN_finalize(&stack_machine);
        return E_MISSED_OPERATOR;
    }
    RPN_finalize(&stack_machine);
    printf("un_s = %c\n", *(expr->curpointer));
    return E_UNEXPECTED_SYMBOL;
}

int init_expression(Expression *expr, char *input) {
    expr->string_form = strdup(input);
    if (expr->string_form == NULL) return E_MEM_ALLOC;
    expr->curpointer = expr->string_form;
    expr->v_tab = malloc(sizeof(Var_table));
    if (expr->v_tab == NULL) {
        free(expr->string_form);
        return E_MEM_ALLOC;
    }
    expr->v_tab->var_num = 0;
    expr->v_tab->vars = NULL;
    return 0;
}

void finalize_expression(Expression *expr) {
    free(expr->string_form);
    for (int i = 0; i < expr->v_tab->var_num; ++i) {
        free(expr->v_tab->vars[i].data);
    }
    free(expr->v_tab->vars);
    free(expr->v_tab);
}