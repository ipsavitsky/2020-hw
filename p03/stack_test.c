#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

int main(void){
    Stack stack;
    stack_init(&stack, 100);
    int test1 = {1, 2, 3};
    stack_push(&stack, test1, 3 * sizeof(int));
    stack_push(&stack, "12345", 5);
    stack_push(&stack, "678910", 6);
    char *str1 = calloc(100, sizeof(char));
    char *str2 = calloc(100, sizeof(char));
    stack_pop(&stack, str1, 100);
    stack_pop(&stack, str2, 100);
    printf("%s\n%s\n", str1, str2);
    int *test_pop = malloc(3 * sizeof(int));
    
    stack_finalize(&stack);
    free(str1);
    free(str2);
    return 0;
}