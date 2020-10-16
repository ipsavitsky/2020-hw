#include <stdio.h>
#include <string.h>

/*you can never overflow a buffer if you have no buffer!*/

void parse(int argc, char *argv[], int mode){
    for (int i = 1; i < argc; ++i) {
        if ((argv[i][0] == '-') && (strlen(argv[i]) > 1)) {
            if (strcmp("--", argv[i]) == 0) {
                if(mode == 1)
                    break;
                else if(mode == -1){
                    for (int j = i + 1; j < argc; ++j) {
                        printf("%s ", argv[j]);
                        ++i;
                    }
                }
            }
            else if (strstr("-a-X-C-h", argv[i])) {
                if(mode == 1){
                    printf("%s %s ", argv[i], argv[i + 1]);
                }
                ++i;
            } 
            else if(mode == 1)
                printf("%s ", argv[i]);
        }
        else if(mode == -1)
            printf("%s ", argv[i]);
    }
}

int main(int argc, char *argv[]) {
    printf("%s ", argv[0]);
    parse(argc, argv, 1);
    printf("-- ");
    parse(argc, argv, -1);
    printf("\n");
    return 0;
}