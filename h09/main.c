#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SAFE(call)          \
    if (call == -1) {       \
        perror("SAFE(): "); \
        exit(1);            \
    }

// specification1: the keywords are list and vector

int main(void) {
#if !(list || vector)
#error no viable compiler options passed
#endif

#if (list && vector)
#error list and vector exclude one another
#endif

    // path to gcc determined by which gcc
    const char *gcc_path = "/usr/bin/gcc";
    int status;
    pid_t pid;
    if ((pid = fork()) == 0) {
#ifdef list
        SAFE(execl(gcc_path, "gcc", "1.c", "-o", "1.exe", "-lm", NULL))

#elif vector
        char *args[] = {"gcc", "1.c", "-o", "1.exe", "-lm", NULL};
        SAFE(execv(gcc_path, args))
#endif
    } else if (pid == -1) {
        fprintf(stderr, "failed to create process\n");
        return 1;
    } else {
        wait(&status);
        // printf("compiled!\n");
    }

    return 0;
}
