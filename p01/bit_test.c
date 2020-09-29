#include <stdio.h>

void bitprint_short(short input){
    unsigned short mask = 1 << 15;
    while(mask != 0){
        // printf("%d\n", mask);
        printf("%d", (input & mask) == 0 ? 0 : 1);
        mask = mask >> 1;
    }
}

void bitprint_int(int input){
    unsigned int mask = 1 << 31;
    while (mask != 0){
        // printf("%d", mask);
        printf("%d", (input & mask) == 0 ? 0 : 1);
        mask = mask >> 1;
    }
}

int main(void){
    unsigned int k = 4294907295;
    printf("short = %ld bytes\n", sizeof(short));
    printf("int = %ld bytes\n", sizeof(int));
    // bitprint_short(k);
    bitprint_int(k);
    printf("\n");
    // bitprint_int((unsigned int)k);
    bitprint_short((unsigned short) k);
    printf("\n");
    return 0;
}

