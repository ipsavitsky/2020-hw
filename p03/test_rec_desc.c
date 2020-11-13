#include "rec_desc.h"
// #include "errors.h"
#include <stdio.h>

int main(void){
    Expression expr;
    int flag;
    // ( 2 +  789.789 ) * e +  avds - 78 + (-8) + 64 * (1 + (-1)) + 235 * (7 + 7 * (7 + 7 * (7 + (-7))))/ abc
    init_expression(&expr, "7  -.5");
    add_variable_to_table(&expr, "avds", 15);
    add_variable_to_table(&expr, "e", 2.71);
    add_variable_to_table(&expr, "abc", 3);
    printf("init: %s\n", expr.string_form);
    double res;
    if((flag = compute_expression(&expr, &res)) != 0){
        err_print(flag);
    }
    printf("result: %lf\n", res);
    finalize_expression(&expr);
}
