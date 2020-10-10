//clarification 1: sum of 20 ints does not overflow int for both x and y
//clarification 2: numbers are considered integer in this task 

#include <stdio.h>

int calculate(int *x, int *y) {
    int sum_x = 0, sum_y = 0, ctrl_sum = 0, i;
    for (i = 0; i < 20; i++) {
        sum_x += x[i] * x[i];
        sum_y += (i > 8) ? y[i] * y[i] : 0;
        ctrl_sum += (i < 15) ? x[i] * y[i] : 0;
    }
    return (ctrl_sum > 0) ? sum_x : sum_y;
}

int main(void) {
    int x[20], y[20], i;
    printf("input x\n");
    for (i = 0; i < 20; i++) {
        scanf("%d", &x[i]);
    }
    printf("input y\n");
    for (i = 0; i < 20; i++) {
        scanf("%d", &y[i]);
    }
    printf("calculated: %d\n", calculate(x, y));
    return 0;
}