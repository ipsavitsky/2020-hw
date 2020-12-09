#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 2048
#endif

#define SAFE_ERRNO(call, func)                                        \
    do {                                                              \
        if ((call) == -1) {                                           \
            char *err = strerror(errno);                              \
            if (write(STDERR_FILENO, func, strlen(func)) == -1) {     \
            }                                                         \
            if (write(STDERR_FILENO, ": ", 2 * sizeof(char)) == -1) { \
            }                                                         \
            if (write(STDERR_FILENO, err, strlen(err)) == -1) {       \
            }                                                         \
            if (write(STDERR_FILENO, "\n", sizeof(char)) == -1) {     \
            }                                                         \
            exit(1);                                                  \
        }                                                             \
    } while (0)

int copyfile(int fd_to, int fd_from) {
    char buf[BUF_SIZE];
    ssize_t nread;
    int saved_errno;
    while ((nread = read(fd_from, buf, sizeof buf)) > 0) {
        char *out_ptr = buf;
        ssize_t written;
        do {
            written = write(fd_to, out_ptr, nread);
            if (written >= 0) {
                nread -= written;
                out_ptr += written;
            } else if (errno != EINTR) {
                saved_errno = errno;
                close(fd_from);
                if (fd_to >= 0) close(fd_to);
                errno = saved_errno;
                return -1;
            }
        } while (nread > 0);
    }

    if (nread == 0) {
        if (close(fd_to) < 0) {
            fd_to = -1;
            saved_errno = errno;
            close(fd_from);
            if (fd_to >= 0) close(fd_to);
            errno = saved_errno;
            return -1;
        }
        close(fd_from);

        /* Success! */
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        char cur_buf[] = "not enough arguments!\n";
        if(write(STDERR_FILENO, cur_buf, sizeof(cur_buf)) == -1){
            exit(1);
        }
        exit(1);
    }
    struct stat sb1, sb2;
    SAFE_ERRNO(stat(argv[argc - 1], &sb1), "stat()");
    if (S_ISREG(sb1.st_mode)) {
        int dest_fd, source_fd;
        if (argc != 3) {
            char cur_buf[] = "last argument is not a directory\n";
            if(write(STDERR_FILENO, cur_buf, sizeof(cur_buf)) == -1){
                exit(1);
            }
            // fprintf(stderr, "last argument is not a directory\n");
            exit(1);
        }
        SAFE_ERRNO(stat(argv[argc - 1], &sb2), "stat()");
        if (!S_ISREG(sb2.st_mode)) {
            char cur_buf[] = "first argument is a directory\n";
            if(write(STDERR_FILENO, cur_buf, sizeof(cur_buf)) == -1){
                exit(1);
            }
        }
        if ((sb1.st_ino == sb2.st_ino) && (sb1.st_dev == sb2.st_dev)) {
            char cur_buf[] = "copying the same file to itself\n";
            if(write(STDERR_FILENO, cur_buf, sizeof(cur_buf)) == -1){
                exit(1);
            }
            exit(1);
        }
        SAFE_ERRNO(dest_fd = creat(argv[argc - 1], 0644), "creat()");
        SAFE_ERRNO(source_fd = open(argv[argc - 2], O_RDONLY), "open()");
        copyfile(dest_fd, source_fd);
    } else if (S_ISDIR(sb1.st_mode)) {
        char cur_buf[] = "second argument is a directory\n";
        if(write(STDERR_FILENO, cur_buf, sizeof(cur_buf)) == -1){
            exit(1);
        }
    } else {
        char cur_buf[] = "only regular files can be copied\n";
        if(write(STDERR_FILENO, cur_buf, sizeof(cur_buf)) == -1){
            exit(1);
        }
    }
}