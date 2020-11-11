#include <signal.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

void track_policy(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
    while (!WIFEXITED(status)) {
        struct user_regs_struct state;

        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        waitpid(pid, &status, 0);
        if(WIFSIGNALED(status)) return;

        // at syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, pid, 0, &state);
            // printf("SYSCALL %lld\n", state.orig_rax);
            long long int rax = state.orig_rax;

            if (rax == __NR_execve) {
                printf("process %d tried to execute a binary! killing process!\n", pid);
                state.rax = __NR_kill;
                state.rdi = pid;
                state.rsi = SIGKILL;
                ptrace(PTRACE_SETREGS, pid, 0, &state);
                ptrace(PTRACE_CONT, pid, 0, 0);
            }

            // if (rax == __NR_openat) {
            //     printf("process %d tried to open a file! killing process!\n", pid);
            //     kill(pid, SIGKILL);
            //     return;
            // }

            // skip after syscall
            // printf("waiting for pid: %d\n", pid);
            ptrace(PTRACE_SYSCALL, pid, 0, 0);
            waitpid(pid, &status, 0);
            if (WIFSIGNALED(status)) return;

            ptrace(PTRACE_GETREGS, pid, 0, &state);
            if (state.orig_rax == __NR_clone) {
                pid_t sys_res = state.rax;
                printf("child process pid %d\n", sys_res);
                ptrace(PTRACE_ATTACH, sys_res, 0, 0);
                printf("attached %d to %d\n", getpid(), sys_res);
                track_policy(sys_res);
            }
        }
    }
    return;
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("specify a program you want to run!\n");
        return 1;
    }
    pid_t pid = fork();
    if (pid != 0) {
        printf("starting tracking process on %d\n", getpid());
        track_policy(pid);
    } else {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execv(argv[1], &(argv[1]));
        perror("execl");
    }
    return 0;
}