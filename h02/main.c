#include <stdio.h>
#include <string.h>

enum
{
    MAXLEN = 256
};

int main(void)
{
    char str[MAXLEN];
    int i;
    char tmp;

    fgets(str, MAXLEN, stdin);
    int len = strlen(str);
    if (str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
    // printf("len = %d\n", len);
    for(i = 0; i < (len - 1) / 2; i++){
        // printf("%c\n%d\n",  str[len - i - 2], len - i - 2);
        tmp = str[i];
        str[i] = str[len - i - 2];
        str[len - i - 2] = tmp;
    }

    printf("%s\n", str);
}
