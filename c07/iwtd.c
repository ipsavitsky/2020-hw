#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHECKSTATUS(status) \
    if (WIFEXITED(status) && (WEXITSTATUS(status) != 0)) perror("")

int main(int argc, char *argv[]) {
    pid_t pid1, pid2;
    int status;
    if ((pid1 = fork()) != -1) {
        if (pid1 == 0) {
            // printf("i am the first child\n");
            execlp(argv[1], argv[1], NULL);
        } else {
            // printf("luke! i am your fuzzer\n");
            wait(NULL);
            // printf("the wait is over\n");
            CHECKSTATUS(status);
            if ((pid2 = fork()) != -1) {
                if (pid2 == 0) {
                    // printf("i am the second child\n");
                    execvp(argv[2], &argv[2]);
                } else {
                    // wait(&status);
                    // CHECKSTATUS(status);
                }
            }
        }
    }
    // FILE *f;
    // f = fopen("iwtd.c", "r");
    // char k;
    // fread(&k, sizeof(char), 1, f);
    return 0;
}