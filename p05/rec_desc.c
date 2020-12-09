#include "arithm_func.h"

// #include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE(call)                           \
    do {                                     \
        if ((flag = call) != 0) return flag; \
    } while (0)

/**
 * add an element(a function/constamt or variable to RPN)
 * \param expression
 * \param size size of data(in bytes) + sizeof(Calculate_elem)
 * \param data data to be written to RPN
 * \param func function to be executed on data
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 */
int put_elem_in_RPN(RPN *expression, Size_elem size, void *data, Calculate_elem func);

/**
 * parse the first layer of operations(+ or -)
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 *
 */
int parse_logic(Expression *expr, RPN *stack_mach);

/**
 * parse the second layer of operations(* or /)
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 *
 */
int parse_pipes(Expression *expr, RPN *stack_mach);

/**
 * parse the third layer(braces/constants/variables)
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 *
 */
int parse_fact(Expression *expr, RPN *stack_mach);

/**
 * parse the fourth layer - variables
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 */
int parse_variable(Expression *expr, RPN *stack_mach);

/**
 * parse the fourth layer - constants
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 */
int parse_literal(Expression *expr, RPN *stack_mach);

int put_elem_in_RPN(RPN *expression, Size_elem size, void *data, Calculate_elem func) {
    if (expression->occupied + sizeof(Size_elem) + size >= expression->data_size) {
        return E_OVERFLOW;
    }
    struct input_data {
        Size_elem size;
        Calculate_elem f;
    };
    struct input_data dat;
    printf("input size = %d\n", size);
    dat.size = size;
    if (dat.size % 8 != 0) dat.size += (8 - size % 8);
    dat.f = func;
    // printf("{%d; %p}\n", dat.size, dat.f);
    memcpy((struct input_data *)&(((char *)expression->data)[expression->occupied]), &dat,
           sizeof(struct input_data));
    expression->occupied += sizeof(struct input_data);
    printf("about to write to rpn: %s\n", (char *)data);
    printf("size: %d\n", size);
    printf("size with padding: %d\n", dat.size);
    if (data != NULL) {
        memcpy((char *)expression->data + expression->occupied, data, size);
        expression->occupied += dat.size;
    }
    return 0;
}

int parse_logic(Expression *expr, RPN *stack_mach) {
    int flag;
    SAFE(parse_pipes(expr, stack_mach));
    char operation;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    while ((strncmp(expr->curpointer, "&&", 2) == 0) || (strncmp(expr->curpointer, "||", 2) == 0) ||
           *(expr->curpointer) == ';') {
        operation = *(expr->curpointer++);
        SAFE(parse_pipes(expr, stack_mach));
        switch (operation) {
            case '&':
                SAFE(put_elem_in_RPN(stack_mach, 0, NULL, sum_double));
                break;
            case '|':
                SAFE(put_elem_in_RPN(stack_mach, 0, NULL, sub_double));
                break;
            case ';':
                SAFE(put_elem_in_RPN(stack_mach, 0, NULL, skip_exec));
                break;
            default:
                break;
        }
    }
    return 0;
}

int parse_pipes(Expression *expr, RPN *stack_mach) {
    int flag;
    SAFE(parse_fact(expr, stack_mach));
    int operation;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    while (strncmp(expr->curpointer, "|", 1) || strncmp(expr->curpointer, ">", 1) ||
           strncmp(expr->curpointer, "<", 1) || strncmp(expr->curpointer, ">>", 2)) {
        // operation = *(expr->curpointer++);
        if (*(expr->curpointer) == "|") {
            operation = 0;
        } else if (*(expr->curpointer) == ">") {
            if (*(expr->curpointer + 1) == ">") {
                operation = 1;
            }
            else{
                operation = 2;
            }
        } else if (*(expr->curpointer) == "<") {
            operation = 3;
        }
        SAFE(parse_fact(expr, stack_mach));
        switch (operation) {
            case 0:
                SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL, mult_double));
                break;
            case 1:
                SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL, out2file_trunc));
                break;
            case 2:
                SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL, out2file));
                break;
            case 3:
                SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL, div_double));
                break;
            default:
                break;
        }
    }
    return 0;
}

int parse_fact(Expression *expr, RPN *stack_mach) {
    int flag;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    if (*(expr->curpointer) == '(') {
        ++(expr->curpointer);
        SAFE(parse_logic(expr, stack_mach));
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
        // printf("un_s = %c\n", *(expr->curpointer));
        return E_UNEXPECTED_SYMBOL;
    }
    return 0;
}

int parse_variable(Expression *expr, RPN *stack_mach) {
    int flag;
    int size = 0;
    unsigned char var[129];
    while (((*(expr->curpointer) != '|') && (*(expr->curpointer) != ';') &&
            (*(expr->curpointer) != '\0')) &&
           (size < 128))
        var[size++] = *(expr->curpointer++);
    var[size - 1] = '\0';
    printf("putting %s(%d)\n", var, size);
    SAFE(put_elem_in_RPN(stack_mach, size, var, prog2stack));
    return 0;
}

int compute_expression(Expression *expr, int *res) {
    RPN stack_machine;
    int flag;
    struct input_data {
        Size_elem size;
        Calculate_elem f;
    };
    size_t rpn_estimate = strlen(expr->string_form) * (sizeof(struct input_data) + sizeof(double));
    SAFE(RPN_init(&stack_machine, rpn_estimate));
    int ret = 0;

    if ((flag = parse_logic(expr, &stack_machine)) != 0) {
        RPN_finalize(&stack_machine);
        return flag;
    }
    put_elem_in_RPN(&stack_machine, 0, NULL, execute);
    // this realloc will never do anything because we demand a smaller chunk of memory so it will
    // always return the same pointer therefore there will never be an error
    stack_machine.data = realloc(stack_machine.data, stack_machine.occupied);
    stack_machine.data_size = stack_machine.occupied;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);

    if ((*(expr->curpointer) == '\0')) {
        printf("might fuck around and compute this bitch\n");
        if ((flag = RPN_compute(&stack_machine, &ret, sizeof(ret), expr->v_tab)) != 0) {
            printf("fuck!\n");
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
    // printf("un_s = %c(%d)\n", *(expr->curpointer), *(expr->curpointer));
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
