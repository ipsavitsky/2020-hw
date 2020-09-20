#include <stdio.h>

int check(void){
    printf("lazy evaluation DOES NOT WORK!! ABANDON SHIP!!!\n");
    return 1;
}

int main(void){
    printf("%d\n", 0 && check());
    return 0;
}