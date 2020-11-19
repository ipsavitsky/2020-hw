#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void){
    int fd[2];
    int safe;
    // create pipes for process communication
    if(pipe(fd) == -1){
        perror("pipe()");
        exit(errno);
    }
    pid_t pid = fork();
    if(pid == -1){
        // on fork() fail
        perror("fork()");
        exit(errno);
    }
    else if(pid == 0){
        safe = dup(STDOUT_FILENO);
        // send output to the pipe
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
        execlp("cat", "cat", "/etc/passwd", NULL);
        // executed only on exec fail
        perror("execl()");
        dup2(safe, STDOUT_FILENO);
        exit(errno);
    }
    else{
        safe = dup(STDIN_FILENO);
        // recieve input from pipe
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("grep", "grep", "user", NULL);
        // executed only on exec fail
        perror("execl()");
        dup2(safe, STDIN_FILENO);
        exit(errno);
    }
}