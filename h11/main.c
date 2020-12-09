#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int fd[2];
    int safe;
    // create pipes for process communication
    if (pipe(fd) == -1) {
        perror("pipe()");
        exit(errno);
    }
    pid_t pid = fork();
    if (pid == -1) {
        // on fork() fail
        perror("fork()");
        exit(errno);
    } else if (pid == 0) {
        safe = dup(STDOUT_FILENO);
        // send output to the pipe
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
        execlp("cat", "cat", "/etc/passwd", NULL);
        // executed only on exec fail
        perror("execl()");
        kill(getppid(), SIGPIPE);
        // retrieving the file descritpors here is unnecessary because everyting here happens inside
        // a process
        dup2(safe, STDOUT_FILENO);
        exit(errno);
    } else {
        safe = dup(STDIN_FILENO);
        // recieve input from pipe
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("grep", "grep", "user", NULL);
        // executed only on exec fail
        perror("execl()");
        kill(pid, SIGPIPE); 
        dup2(safe, STDIN_FILENO);
        exit(errno);
    }
}


// this should work but for some reason does not
// second process is needed because it more accurately represents what happens in the shell
/*
int main(void) {
    pid_t chld1, chld2;
    int safe, fd[2];
    if(pipe(fd) == -1){
        perror("pipe()");
        exit(1);
    }
    if ((chld1 = fork()) == 0) {
        safe = dup(STDIN_FILENO);
        // recieve input from pipe
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("grep", "grep", "user", NULL);
        // executed only on exec fail
        perror("execl()");
        dup2(safe, STDIN_FILENO);
        _exit(110);  // unused code to indicate exec() fail
    } else if (chld1 == -1) {
        perror("fork()");
        _exit(1);
    }
    if ((chld2 = fork()) == 0) {
        safe = dup(STDOUT_FILENO);
        // send output to the pipe
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
        execlp("cat", "cat", "/etc/passwd", NULL);
        // executed only on exec fail
        perror("execl()");
        dup2(safe, STDOUT_FILENO);
        _exit(110);  // unused code to indicate exec() fail
    } else if (chld2 == -1) {
        perror("fork()");
        _exit(110);
    }

    int status;
    pid_t exitpid = wait(&status);
    pid_t scnd_chld = (exitpid == chld1) ? chld2 : chld1;
    if (WIFEXITED(status) && (WEXITSTATUS(status) > 100)) {
        kill(scnd_chld, SIGKILL);
        // (exitpid == chld1) ? chld2 : chld1;
        exit(1);
    }
    waitpid(scnd_chld, NULL, 0);
}
*/