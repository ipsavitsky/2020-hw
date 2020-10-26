#include <stdio.h>
#include <string.h>

char password[] = "PASSWORD";

int main(void) {
    printf("Enter the password: ");
    char check[sizeof password + 2] = {0};
    fgets(check, sizeof check, stdin);
    {
        size_t len = strlen(check);
        if (check[len - 1] == '\n') {
            check[len - 1] = '\0';
        }
    }

    if (strcmp(password, check) == 0) {
        printf("You win :(\n");
    } else {
        printf("You looooose!!!!\n");
    }
}