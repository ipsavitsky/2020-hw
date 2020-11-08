#include "rec_desc.h"
// #include "errors.h"
#include <stdio.h>

int main(void){
    Expression expr;
    int flag;
    // (2+789.789)*e+avds-78+(-8)+64*(1+(-1))+235*(7+7*(7+7*(7+(-7))))
    init_expression(&expr, "(2+789.789)*e+avds-78+(-8)+64*(1+(-1))+235*(7+7*(7+7*(7+(-7))))");
    printf("init: %s\n", expr.string_form);
    double res;
    if((flag = compute_expression(&expr, &res)) != 0){
        err_print(flag);
    }
    printf("result: %lf\n", res);
    finalize_expression(&expr);
}