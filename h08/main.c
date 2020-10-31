#include <math.h>
#include <stdio.h>
#include <string.h>

// specialization1: this program should be compiled with a modern version of
// gcc(for inline struct assignment and an iterator declaration in for)

// specialization2: in case of incorrect a, b and h values(e.g. b < a or h >
// b-a) the program will output nothing

// specialization3: the input is always correct(a, b and h as well as the
// function name are always present)

// specialization4: in case of an incorrect
// value(e.g.negative number as argument to sqrt()) nan/inf macros is used

typedef double (*tf)(double);

typedef struct {
    const char *str;
    tf f_p;
} func;

int main(int argc, char *argv[]) {
    func funcs[7];
    funcs[0] = (func){.str = "sin", .f_p = sin};
    funcs[1] = (func){.str = "cos", .f_p = cos};
    funcs[2] = (func){.str = "tan", .f_p = tan};
    funcs[3] = (func){.str = "log", .f_p = log};
    funcs[4] = (func){.str = "fabs", .f_p = fabs};
    funcs[5] = (func){.str = "sqrt", .f_p = sqrt};
    funcs[6] = (func){.str = "exp", .f_p = exp};
    double a, b, h;
    int i;
    for (i = 0; i < 7; i++)
        if (!strcmp(argv[1], funcs[i].str)) break;
    if (i == 7) {
        printf("Имя функции задано неверно\n");
        return -1;
    }
    sscanf(argv[2], "%lf", &a);
    sscanf(argv[3], "%lf", &b);
    sscanf(argv[4], "%lf", &h);
    for (double cur = a; cur <= b; cur += h)
        printf("%s(%f)=%f\n", funcs[i].str, cur, funcs[i].f_p(cur));
    return 0;
}