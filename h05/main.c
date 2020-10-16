// clarification 1: sum of squares of 20 ints does not overflow int for either x
// or y

// clarification 2: long long int allows for far more posibilities of sequences,
// but it's best to be sure and still follow the first rule

// clarification 3: input numbers are considered integer in this task

#include <stdio.h>

long long int calculate(int *x, int *y) {
    long long int sum_x = 0, sum_y = 0, ctrl_sum = 0;
    unsigned short i;
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
    printf("calculated: %lld\n", calculate(x, y));
    return 0;
}