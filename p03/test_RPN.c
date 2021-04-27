#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RPN.h"

int add_double(const void *elem, Size_elem size, Stack *stack) {
    return stack_push(stack, &(((char *)elem)[sizeof(Calculate_elem)]),
                      size - sizeof(Calculate_elem));
}

int sum_double(const void *elem, Size_elem size, Stack *stack) {
    double elem1, elem2;
    stack_pop(stack, &elem1, sizeof(elem1));
    stack_pop(stack, &elem2, sizeof(elem2));
    double res = elem1 + elem2;
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int mult_double(const void *elem, Size_elem size, Stack *stack) {
    double elem1, elem2;
    stack_pop(stack, &elem1, sizeof(elem1));
    stack_pop(stack, &elem2, sizeof(elem2));
    double res = elem1 * elem2;
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int main(void) {
    RPN expression;
    int cur = 0;
    Calculate_elem *func_ptr;

    RPN_init(&expression, 200);

    *((char *)expression.data) = sizeof(double) + sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = add_double;
    cur += sizeof(*func_ptr);
    double add_1 = 123;
    memcpy(&(((char *)expression.data)[cur]), &add_1, sizeof(add_1));
    cur += sizeof(add_1);

    printf("added %f to stackmachine to %d\n", add_1, cur);

    ((char *)expression.data)[cur] = sizeof(double) + sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = add_double;
    cur += sizeof(*func_ptr);
    double add_2 = 15;
    memcpy(&(((char *)expression.data)[cur]), &add_2, sizeof(add_2));
    cur += sizeof(add_2);

    printf("added %f to stackmachine to %d\n", add_2, cur);

    ((char *)expression.data)[cur] = sizeof(double) + sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = add_double;
    cur += sizeof(*func_ptr);
    double add_3 = 6;
    memcpy(&(((char *)expression.data)[cur]), &add_3, sizeof(add_3));
    cur += sizeof(add_3);

    printf("added %f to stackmachine to %d\n", add_3, cur);

    ((char *)expression.data)[cur] = sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = sum_double;
    cur += sizeof(Calculate_elem);

    printf("added addition function to %d\n", cur);

    ((char *)expression.data)[cur] = sizeof(double) + sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = add_double;
    cur += sizeof(*func_ptr);
    double add_4 = 4.5;
    memcpy(&(((char *)expression.data)[cur]), &add_4, sizeof(add_3));
    cur += sizeof(add_4);

    printf("added %f to stackmachine to %d\n", add_4, cur);

    ((char *)expression.data)[cur] = sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = mult_double;
    cur += sizeof(Calculate_elem);

    printf("added multiplication function to %d\n", cur);

    ((char *)expression.data)[cur] = sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = sum_double;
    cur += sizeof(Calculate_elem);

    printf("added addition function to %d\n", cur);

    expression.data = realloc(expression.data, cur);
    expression.data_size = cur;

    void *res = calloc(100, sizeof(char));

    RPN_compute(&expression, res, 100);
    printf("%lf\n", *((double *)res));
    RPN_finalize(&expression);
    free(res);
}
