#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "rec_desc.h"

int main(void) {
    char *curline = NULL;
    int res;
    size_t what;
    int flag;
    Expression expr;
    init_expression(&expr, "");
    // char *cwd;
    for (;;) {
        // cwd = getcwd(NULL, 0);
        printf(">>> ");
        if (getline(&curline, &what, stdin) == -1) {
            printf("error reading\n");
            break;
        }
        else if ((strlen(curline) == 2) && (*curline == 'q')){
            break;
        } else {
            free(expr.string_form);
            expr.string_form = strdup(curline);
            expr.curpointer = expr.string_form;
            if ((flag = compute_expression(&expr, &res)) != 0) {
                err_print(flag);
            }
            printf("result = %d\n", res);
        }
        res = 0;
    }
    // free(cwd);
    free(curline);
    finalize_expression(&expr);
    return 0;
}