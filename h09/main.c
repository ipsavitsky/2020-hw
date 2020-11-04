#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define SAFE(call)          \
    if (call == -1) {       \
        perror("SAFE(): "); \
        exit(1);            \
    }


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

    if (fork() == 0) {
#ifdef list
        SAFE(execl(gcc_path, "gcc", "1.c", "-o", "1.exe", "-lm", NULL))

#elif vector
        char *args[] = {"gcc", "1.c", "-o", "1.exe", "-lm", NULL};
        SAFE(execv(gcc_path, args))
#endif
    } else {
        wait(&status);
        //printf("compiled!\n");
    }

    return 0;
}
