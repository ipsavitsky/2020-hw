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

int parse_fact(Expression *expr, double *res);
int parse_product(Expression *expr, double *res);
int parse_sum(Expression *expr, double *res);
int parse_number(Expression *expr, double *res);



int init_expression(Expression *expr, char *input) {
    expr->string_form = strdup(input);
    if (expr->string_form == NULL)
        return E_MEM_ALLOC;
    expr->curpointer = expr->string_form;
    return 0;
}

int parse_sum(Expression *expr, double *res) {
    double fact1, fact2;
    parse_product(expr, &fact1);
    *res = fact1;
    while (*(expr->curpointer) == '+') {
        expr->curpointer++;
        parse_product(expr, &fact2);
        *res += fact2;
    }
    return 0;
}

int parse_product(Expression *expr, double *res) {
    double fact1 = 0, fact2 = 0;
    parse_fact(expr, &fact1);
    *res = fact1;
    while (*(expr->curpointer) == '*') {
        expr->curpointer++;
        parse_fact(expr, &fact2);
        *res *= fact2;
    }
    return 0;
}

int parse_fact(Expression *expr, double *res) {
    double ret;
    if ((*(expr->curpointer) > '0') && (*(expr->curpointer) < '9'))
        parse_number(expr, &ret);
    else if (*(expr->curpointer) == '(') {
        expr->curpointer++;
        parse_sum(expr, &ret);
        // TODO: check for )
        expr->curpointer++;
    } else
        return E_UNEXPECTED_SYMBOL;
    *res = ret;
    return 0;
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
    parse_sum(expr, res);
    if (*(expr->curpointer) == '\0') return 0;
    printf("something went wrong\n");
    return E_UNEXPECTED_SYMBOL;
}