#include "arithm_func.h"

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SAFE(call)                           \
    do {                                     \
        if ((flag = call) != 0) return flag; \
    } while (0)

int prog2stack(Calculation_data *data) { return stack_push(data->stack, data->elem, data->size); }

int skip_exec(Calculation_data *data) {
    int useless, flag;
    SAFE(stack_pop(data->stack, &useless, sizeof(useless)));
    return 0;
}

int execute(Calculation_data *data) {
    pid_t pid;
    int stat, flag;
    char *curpars;
    // FIXME: this is gross
    char *args[15];
    char full_call[128];
    SAFE(stack_pop(data->stack, full_call, sizeof(full_call)));
    int arg_cnt = 0;
    curpars = full_call;
    while (1) {
        if (*curpars == '\0') break;
        args[arg_cnt++] = curpars;
        while ((!isspace(*curpars)) && (*curpars != '\0')) curpars++;
        if (*curpars == '\0') break;
        *curpars = '\0';
        curpars++;
        while (isspace(*curpars)) curpars++;
    }
    for (int i = 0; i < arg_cnt; ++i) {
        printf("%s(%ld)\n", args[i], strlen(args[i]));
    }
    args[arg_cnt] = NULL;
    if ((pid = fork()) == 0) {
        execvp(args[0], args);
        // perror("execve()");
        return E_UNKNOWN_EXEC;
    }
    while (1) {
        waitpid(pid, &stat, 0);
        if (WIFEXITED(stat) == 1) {
            int status = WEXITSTATUS(stat);
            SAFE(stack_push(data->stack, &status, sizeof(status)));
            break;
        }
    }
    printf("program executed\n");
    return 0;
}