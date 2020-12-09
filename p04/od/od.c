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

void process_file(int cur_file, size_t *global_pos, int vflag){
    ssize_t nread;
    char buf[BUF_SIZE];
    while (nread = read(cur_file, buf, sizeof(buf)), nread > 0) {
        for (int j = 0; j < nread; ++j) {
            int size;
            if ((*global_pos - 1) % 10 == 0) {
                char curbuf[9];
                size = sprintf(curbuf, "%06ld ", *global_pos);
                SAFE_ERRNO(write(STDOUT_FILENO, curbuf, size), "write()");
            }
            char tmp_buf[13];
            tmp_buf[0] = '\\';
            tmp_buf[2] = ' ';
            switch (buf[j]) {
                case '\t':
                    tmp_buf[1] = 't';
                    size = 3;
                    break;
                case '\n':
                    tmp_buf[1] = 'n';
                    size = 3;
                    break;
                case '\r':
                    tmp_buf[1] = 'r';
                    size = 3;
                    break;
                case '\v':
                    tmp_buf[1] = 'v';
                    size = 3;
                    break;
                default:
                    tmp_buf[0] = buf[j];
                    tmp_buf[1] = ' ';
                    size = 2;
                    break;
            }
            SAFE_ERRNO(write(STDOUT_FILENO, tmp_buf, size), "write()");
            if(vflag > 0){
                sprintf(tmp_buf, "%03o ", buf[j]);
                size = strlen(tmp_buf);
                SAFE_ERRNO(write(STDOUT_FILENO, tmp_buf, size), "write()");
            }
            if ((*global_pos - 1) % 10 == 9) {
                SAFE_ERRNO(write(STDOUT_FILENO, "\n", 1), "write()");
            }
            ++(*global_pos);
        }
    }
}


int main(int argc, char *argv[]) {
    // argp_parse();
    int cur_file = -1, vflag = 0;
    size_t global_pos = 1;
    for(int i = 1; i < argc; ++i)
        if(strcmp("-b", argv[i]) == 0)
            vflag = i;
    if (argc - ((vflag > 0) ? 1 : 0) == 1) {
        // printf("so you have chosen... death\n");
        SAFE_ERRNO(cur_file = dup2(STDIN_FILENO, cur_file), "dup2()");
        // close(cur_file);
        process_file(cur_file, &global_pos, vflag);
    }
    for (int i = 1; i < argc; ++i) {
        if(i == vflag) continue;
        SAFE_ERRNO(cur_file = open(argv[i], O_RDONLY), "open()");
        struct stat sb;
        SAFE_ERRNO(stat(argv[i], &sb), "stat()");
        if(!S_ISREG(sb.st_mode)){
            char curbuf[] = "not a regular file; skipping";
            if(write(STDERR_FILENO, curbuf, sizeof(curbuf)) == -1){
                abort();
            }
            continue;
        }
        process_file(cur_file, &global_pos, vflag);
    }
    SAFE_ERRNO(write(STDOUT_FILENO, "\n", 1), "write()");
}