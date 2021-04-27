#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define SAFE(call)                                           \
    do {                                                     \
        if ((err = call) != 0) {                             \
            printf("caught %d on line %d\n", err, __LINE__); \
        }                                                    \
    } while (0)

int main(void) {
    Stack stack;
    size_t one_frame = sizeof(double) + sizeof(Size_elem);
    double a = 1, b;
    int err;
    // test the underflow error
    stack_init(&stack, one_frame);
    SAFE(stack_push(&stack, &a, sizeof(double)));
    SAFE(stack_pop(&stack, &b, sizeof(double)));
    SAFE(stack_pop(&stack, &b, sizeof(double)));
    stack_finalize(&stack);

    // test the overflow error
    stack_init(&stack, 2 * one_frame);
    SAFE(stack_push(&stack, &a, sizeof(double)));
    SAFE(stack_push(&stack, &a, sizeof(double)));
    SAFE(stack_push(&stack, &a, sizeof(double)));
    stack_finalize(&stack);

    // we can store anyting in this stack!
    stack_init(&stack, 20);
    SAFE(stack_push(&stack, "WORLD!", strlen("WORLD!") + 1));
    SAFE(stack_push(&stack, "HELLO ", strlen("HELLO ") + 1));
    char *word1 = malloc(sizeof(char) * 10);  // 10 is a rough estimate for 7 :)
    char *word2 = malloc(sizeof(char) * 10);
    SAFE(stack_pop(&stack, word1, 10));
    SAFE(stack_pop(&stack, word2, 10));
    printf("%s %s\n", word1, word2);
    free(word1);
    free(word2);
    stack_finalize(&stack);
    return 0;
}