#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

void track_policy(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
    while (!WIFEXITED(status)) {
        struct user_regs_struct state;

        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        pid_t elif;
        elif = wait(&status);

        // waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) || WIFEXITED(status)) continue; //waitpid(pid, &status, 0);

        // at syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, pid, 0, &state);
            // printf("SYSCALL %lld\n", state.orig_rax);

            if (state.orig_rax == __NR_execve) {
                printf("process %d tried to execute a binary! killing process!\n", pid);
                state.rax = __NR_kill;
                state.rdi = pid;
                state.rsi = SIGKILL;
                ptrace(PTRACE_SETREGS, pid, 0, &state);
                ptrace(PTRACE_CONT, pid, 0, 0);
                waitpid(pid, NULL, 0);
                return;
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
            if (WIFSIGNALED(status) || WIFEXITED(status)) return;

            if (state.orig_rax == __NR_clone) {
                ptrace(PTRACE_GETREGS, pid, 0, &state);
                pid_t sys_res = state.rax;
                printf("child process pid %d\n", sys_res);
                if (fork() == 0) {
                    ptrace(PTRACE_ATTACH, sys_res, 0, 0);
                    waitpid(sys_res, &status, 0);
                    ptrace(PTRACE_SETOPTIONS, sys_res, 0, PTRACE_O_TRACESYSGOOD);
                    pid = sys_res;
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
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