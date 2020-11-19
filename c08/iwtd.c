#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define CHECKSTATUS(status) \
    if (WIFEXITED(status) && (WEXITSTATUS(status) != 0)) perror("")

int main(int argc, char *argv[]) {
    // pid_t pid1, pid2;
    // int status;
    if(fork() == 0){
        int fd, k;
        if ((fd = open(argv[2], O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1) {
            fprintf(stderr, "error opening file\n");
            exit(1);
        }
        // if ((k = open(argv[0], O_WRONLY)) == -1) {
        //     fprintf(stderr, "error opening file");
        //     exit(1);
        // }
        // dup2(2, k);
        k = dup(2);
        dup2(fd, 2);
        close(fd);
        dup2(k, 2);
    }
    else{
        wait(NULL);
    }
    return errno;
}
