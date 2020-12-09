#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef SLEEP_DELAY
#define SLEEP_DELAY 5
#endif

void sigint_hndlr(int s) {
    static int call_count = 0;
    ++call_count;
    if (call_count == 2) {
        signal(SIGINT, SIG_DFL);
    }
}

void sigtrp_hndlr(int s) {
    static int call_count = 0;
    ++call_count;
    if (call_count % 2 == 0) {
        int fd[2];
        pipe(fd);
        if (fork() == 0) {
            printf("%d: chld1\n", getpid());
            // have to kill myself so that i won't return to main and duplicate everything
            // this should also take care of any zombies
            raise(SIGKILL);
        }
        if (fork() == 0) {
            printf("%d: chld2\n", getpid());
            // have to kill myself so that i won't return to main and duplicate everything
            raise(SIGKILL);
        }
        close(fd[0]);
        close(fd[1]);
    }
}

int main(void) {
    signal(SIGINT, sigint_hndlr);
    signal(SIGTRAP, sigtrp_hndlr);
    while (1) {
        // kill(getpid(), SIGTRAP);
        sleep(SLEEP_DELAY);
    }
}