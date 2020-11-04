#include "RPN.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_int(const void *elem, Size_elem size, Stack *stack) {
    return stack_push(stack, &(((char *)elem)[sizeof(Calculate_elem)]),
                      size - sizeof(Calculate_elem));
}

int sum_int(const void *elem, Size_elem size, Stack *stack) {
    int elem1, elem2;
    stack_pop(stack, &elem1, sizeof(elem1));
    stack_pop(stack, &elem2, sizeof(elem2));
    int res = elem1 + elem2;
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int mult_int(const void *elem, Size_elem size, Stack *stack) {
    int elem1, elem2;
    stack_pop(stack, &elem1, sizeof(elem1));
    stack_pop(stack, &elem2, sizeof(elem2));
    int res = elem1 * elem2;
    // res = elem1 + elem2;
    // printf("res: %d\n", res);
    stack_push(stack, &res, sizeof(res));
    return 0;
}

int main(void) {
    RPN expression;
    int cur = 0;
    Calculate_elem *func_ptr;

    RPN_init(&expression, 100);


    *((char *)expression.data) = sizeof(int) + sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = add_int;
    cur += sizeof(*func_ptr);
    int add_1 = 2;
    memcpy(&(((char *)expression.data)[cur]), &add_1, sizeof(add_1));
    cur += sizeof(int);

    ((char *)expression.data)[cur] = sizeof(int) + sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = add_int;
    cur += sizeof(*func_ptr);
    int add_2 = 3;
    memcpy(&(((char *)expression.data)[cur]), &add_2, sizeof(add_2));
    cur += sizeof(int);

    ((char *)expression.data)[cur] = sizeof(int) + sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = add_int;
    cur += sizeof(*func_ptr);
    int add_3 = 5;
    memcpy(&(((char *)expression.data)[cur]), &add_3, sizeof(add_3));
    cur += sizeof(int);

    ((char *)expression.data)[cur] = sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = sum_int;
    cur += sizeof(Calculate_elem);

    ((char *)expression.data)[cur] = sizeof(Calculate_elem);
    cur += sizeof(Size_elem);
    func_ptr = (Calculate_elem *)&(((char *)expression.data)[cur]);
    *func_ptr = mult_int;
    cur += sizeof(Calculate_elem);

    expression.data = realloc(expression.data, cur);
    expression.data_size = cur;

    void *res = calloc(100, sizeof(char));

    RPN_compute(&expression, res, 100);
    printf("%d\n", *((int *)res));
    RPN_finalize(&expression);
    free(res);
}
