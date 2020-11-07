#include "rec_desc.h"
#include <stdio.h>

int main(void){
    Expression expr;
    init_expression(&expr, "-(-2)");
    printf("init: %s\n", expr.string_form);
    double res;
    compute_expression(&expr, &res);
    printf("result: %lf\n", res);
    finalize_expression(&expr);
}