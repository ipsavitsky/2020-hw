#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *fp;
    char name[] = "/haha";
    pid_t chld = fork();
    if (chld == 0) {
        if ((fp = fopen(name, "r")) == NULL) {
            printf("Не удалось открыть файл");
            // getchar();
            return 0;
        } else {
            printf("Удалось открыть файл");
        }
        // открыть файл удалось
        fclose(fp);
    }
    return 0;
}
