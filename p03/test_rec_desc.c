#include "rec_desc.h"
#include <stdio.h>

int main(void){
    Expression expr;
    init_expression(&expr, "(2+789.789)*e+avds-78+(-8)+64*(1+(-1))+235*(7+7*(7+7*(7+(-7)))"); //465*76-90/45+30-30/(-4)
    printf("init: %s\n", expr.string_form);
    double res;
    compute_expression(&expr, &res);
    printf("result: %lf\n", res);
    finalize_expression(&expr);
}