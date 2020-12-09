#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

int is_relative(const char *path, size_t size) {
    if (size == 0 || path[0] == '/') {
        return 0;
    }

    int depth = 0;
    const char *p1 = path;
    /*loop invariant:
        p1 points to the character which previous character is /
                            or above &path[size]
        depth == depth of the <&path[0]..p1>*/
    while (p1 - path < size) {
        // p2 points to the nearest / or equals to NULL
        const char *p2 = memchr(p1, '/', path - p1 + size);
        if (p2 == 0) {
            p2 = path + size;
        }
        if (p2 == p1) {
            // don't change depth
        } else if (p2 == p1 + 1 && p1[0] == '.') {
            // don't change depth
        } else if (p2 == p1 + 2 && p1[0] == '.' && p1[1] == '.') {
            --depth;
        } else {
            ++depth;
        }
        if (depth < 0) {
            return 0;
        }
        p1 = p2 + 1;
    }

    return 1;
}

void kill_in_syscall(pid_t pid, struct user_regs_struct *state) {
    state->orig_rax = __NR_kill;
    state->rdi = pid;
    state->rsi = SIGKILL;
    ptrace(PTRACE_SETREGS, pid, 0, state);
    ptrace(PTRACE_CONT, pid, 0, 0);
    waitpid(pid, NULL, 0);
    return;
}

// size_t peek_filename(char *filename, pid_t pid, struct user_regs_struct *state) {
//     union data {
//         char chars[2];
//         unsigned short word;
//     } dat;
//     long long int cur_addr = state->rsi;
//     size_t fn_size = 0;
//     while (1) {
//         dat.word = ptrace(PTRACE_PEEKDATA, pid, cur_addr, 0);
//         if (dat.chars[0] == '\0') {
//             filename[fn_size++] = '\0';
//             break;
//         }
//         // printf("%c%c", dat.chars[0], dat.chars[1]);
//         filename[fn_size++] = dat.chars[0];
//         filename[fn_size++] = dat.chars[1];
//         cur_addr += 2;
//     }
// }

void track_policy(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
    while (!WIFEXITED(status)) {
        struct user_regs_struct state;

        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        if (wait(&status) == -1) return;

        // at syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, pid, 0, &state);
            // printf("SYSCALL %lld\n", state.orig_rax);
            if (state.orig_rax == __NR_openat) {
                ptrace(PTRACE_GETREGS, pid, 0, &state);
                // i tried to make this a fuction but failed
                union data {
                    char chars[2];
                    unsigned short word;
                } dat;
                long long int cur_addr = state.rsi;
                // 1024 is a limit for a filename
                char filename[1024];
                size_t fn_size = 0;
                while (1) {
                    dat.word = ptrace(PTRACE_PEEKDATA, pid, cur_addr, 0);
                    // strings are always null terrminated
                    if (dat.chars[0] == '\0') {
                        filename[fn_size++] = '\0';
                        break;
                    }
                    filename[fn_size++] = dat.chars[0];
                    filename[fn_size++] = dat.chars[1];
                    cur_addr += 2;
                    if (dat.chars[1] == '\0') break;
                }
                if ((strcmp(filename, "/etc/ld.so.cache") != 0) &&
                    (strcmp(filename, "/lib/x86_64-linux-gnu/libc.so.6") != 0) &&
                    !is_relative(filename, fn_size)) {
                    printf("process %d tried to open a file %s\n", pid, filename);
                    kill_in_syscall(pid, &state);
                    return;
                }
            }

            else if ((state.orig_rax == __NR_creat) || (state.orig_rax == __NR_mkdir) ||
                    (state.orig_rax == __NR_unlink) || (state.orig_rax == __NR_rmdir)) {
                ptrace(PTRACE_GETREGS, pid, 0, &state);
                union data {
                    char chars[2];
                    unsigned short word;
                } dat;
                long long int cur_addr = state.rdi;
                char filename[1024];
                size_t fn_size = 0;
                while (1) {
                    dat.word = ptrace(PTRACE_PEEKDATA, pid, cur_addr, 0);
                    if (dat.chars[0] == '\0') {
                        filename[fn_size++] = '\0';
                        break;
                    }
                    // printf("%c%c", dat.chars[0], dat.chars[1]);
                    filename[fn_size++] = dat.chars[0];
                    filename[fn_size++] = dat.chars[1];
                    cur_addr += 2;
                    if (dat.chars[1] == '\0') break;
                }
                if (!is_relative(filename, fn_size)) {
                    printf("process %d: illegal file manipulation at %s", pid, filename);
                    // kill process!
                    kill_in_syscall(pid, &state);
                    return;
                }
            } else if (state.orig_rax == __NR_symlink) {
                printf("process %d tried to create a symbolic link!\n", pid);
                kill_in_syscall(pid, &state);
                return;
            } else if (state.orig_rax == __NR_getcwd) {
                printf("process %d tried to get current working directory!\n", pid);
                kill_in_syscall(pid, &state);
                return;
            }

            else if (state.orig_rax == __NR_chdir) {
                printf("process %d tried to change current working directory!\n", pid);
                kill_in_syscall(pid, &state);
                return;
            } else if (state.orig_rax == __NR_execve) {
                printf("process %d tried to execute a binary! killing process!\n", pid);
                kill_in_syscall(pid, &state);
                return;
            }

            // skip after syscall
            // printf("waiting for pid: %d\n", pid);
            ptrace(PTRACE_SYSCALL, pid, 0, 0);
            if (wait(&status) == -1) return;
            // waitpid(pid, &status, 0);

            // if (WIFSIGNALED(status) || WIFEXITED(status)) continue;

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
        // close standart io streams
        mkdir(argv[1], 0777);
        chdir(argv[1]);
        close(0);
        close(1);
        close(2);
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execv(argv[1], &(argv[1]));
        perror("execv");
    }
    return 0;
}