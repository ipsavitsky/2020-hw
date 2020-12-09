#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 4096
#endif

#define NO_OF_CHARS 256

#define SAFE_ERRNO(call, func)                                        \
    do {                                                              \
        if ((call) == -1) {                                           \
            char *err = strerror(errno);                              \
            if (write(STDERR_FILENO, func, strlen(func)) == -1) {     \
                abort();                                              \
            }                                                         \
            if (write(STDERR_FILENO, ": ", 2 * sizeof(char)) == -1) { \
                abort();                                              \
            }                                                         \
            if (write(STDERR_FILENO, err, strlen(err)) == -1) {       \
                abort();                                              \
            }                                                         \
            if (write(STDERR_FILENO, "\n", sizeof(char)) == -1) {     \
                abort();                                              \
            }                                                         \
            exit(1);                                                  \
        }                                                             \
    } while (0)

int max(int a, int b) { return (a > b) ? a : b; }

void bad_сhar_heur(char *str, int size, int badchar[NO_OF_CHARS]) {
    int i;
    for (i = 0; i < NO_OF_CHARS; i++) badchar[i] = -1;
    for (i = 0; i < size; i++) badchar[(int)str[i]] = i;
}

char *search_substr(char *haystack, char *needle) {
    // FIXME: THIS IS VERY BAD
    int m = strlen(needle);
    int n = strlen(haystack);
    int badchar[NO_OF_CHARS];
    bad_сhar_heur(needle, m, badchar);
    int s = 0;
    while (s <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && needle[j] == haystack[s + j]) j--;
        if (j < 0) {
            // printf("\n pattern occurs at shift = %d", s);
            return haystack + s;
            // s += (s + m < n) ? m - badchar[(int)haystack[s + m]] : 1;
        } else
            s += max(1, j - badchar[(int)haystack[s + j]]);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int in_fd = -1, vflag = 0;  
    for (int i = 1; i < argc; ++i)
        if (strcmp("-v", argv[i]) == 0) vflag = i;
    if (argc - ((vflag > 0) ? 1 : 0) == 2) {
        SAFE_ERRNO(in_fd = dup2(STDIN_FILENO, in_fd), "dup2()");
    } else if (argc - ((vflag > 0) ? 1 : 0) == 3) {
        SAFE_ERRNO(in_fd = open(argv[1 + ((vflag == 1) ? 1 : 0)], O_RDONLY), "open()");
        struct stat sb;
        SAFE_ERRNO(stat(argv[1 + ((vflag == 1) ? 1 : 0)], &sb), "stat()");
        if (!S_ISREG(sb.st_mode)) {
            char curbuf[] = "not a regular file; skipping";
            if (write(STDERR_FILENO, curbuf, sizeof(curbuf)) == -1) {
                abort();
            }
            exit(1);
        }
    }

    char *needle = argv[argc - 1 - ((vflag == argc - 1) ? 1 : 0)];
    char *buf;
    char *concat_ptr;
    buf = malloc(BUF_SIZE);
    concat_ptr = buf;
    ssize_t nread;
    size_t read_sz = BUF_SIZE - (concat_ptr - buf);
    size_t buf_border = BUF_SIZE;
    while ((nread = read(in_fd, concat_ptr, read_sz - 1)) > 0) {
        // printf("in cycle\n");
        // printf("putting terminator in %d\n", nread + (concat_ptr - buf));
        buf[nread + (concat_ptr - buf)] = '\0';
        // printf("new: %s\n", buf);
        if (strchr(buf, '\n') == NULL) {
            buf_border *= 2;
            buf = realloc(buf, buf_border);
            concat_ptr = buf + nread;
            read_sz = buf_border / 2;
            continue;
        }
        if (vflag == 0) {
            char *foundstr = buf;
            char *lborder, *rborder;
            while ((foundstr = search_substr(foundstr, needle)) != NULL) {
                // printf("found entry");
                lborder = rborder = foundstr;
                while ((lborder > buf) && (*lborder != '\n')) lborder--;
                while ((rborder < buf + buf_border) && (*rborder != '\n')) rborder++;
                short offset = 1;
                if (lborder == buf) {
                    offset = 0;
                }
                if (rborder != buf + buf_border) {
                    if (write(STDOUT_FILENO, lborder + offset, rborder - lborder) == -1) {
                        abort();
                    }
                }
                foundstr = rborder;
            }
        } else {
            char *foundstr = buf;
            char *lborder, *rborder;
            lborder = buf;
            while ((foundstr = search_substr(foundstr, needle)) != NULL) {
                rborder = foundstr;
                while ((rborder > buf) && (*rborder != '\n')) rborder--;
                short offset = 1;
                if (lborder == buf) {
                    offset = 0;
                }
                if (lborder != buf + buf_border) {
                    if (write(STDOUT_FILENO, lborder + offset, rborder - lborder) == -1) {
                        abort();
                    }
                }
                lborder = foundstr + 1;
                while ((lborder < buf + buf_border) && (*lborder != '\n')) lborder++;
                foundstr = lborder;
            }
            // TODO: get rid of strlen()
            if (write(STDOUT_FILENO, lborder, strlen(lborder)) == -1) {
                exit(1);
            }
        }

        // TODO: replace with a better variant
        char *last_str = strrchr(buf, '\n');
        if ((last_str != NULL)) {
            ++last_str;
            size_t str_len = strlen(last_str);
            memmove(buf, last_str, str_len);
            concat_ptr = buf + str_len;
            read_sz = buf_border - str_len;
        } else {
            // printf("strrchr() returned NULL!\n");
            concat_ptr = buf;
            read_sz = buf_border;
        }
    }
    free(buf);
}