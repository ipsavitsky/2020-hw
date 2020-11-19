#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// ejudge hates me so i have to write it in one line :(
#define SAFE_OPEN(call)                 \
    if ((call) == -1) {                 \
        printf("error opening file\n"); \
        exit(1);                        \
    }

// specification1: if the input file does not exist the program halts
// specification2: if the output files do not exist, they are created in 0644 mode
// specification3: output files are reset when opened
// specification4: the first output file gets an additional newline at the end(as a present!)

int main(int argc, char *argv[]) {
    int inp, f1, f2;
    if (argc < 3) {
        printf("not enough arguments\n");
        exit(1);
    }
    SAFE_OPEN(inp = open(argv[1], O_RDONLY))
    SAFE_OPEN(f1 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644))
    SAFE_OPEN(f2 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644))
    char cur_ch;
    // specification: 1024 INCLUDES \n\0 at the end(so 1022 sensible characters)
    char buf[1024];
    size_t cur_size;
    size_t line_ctr = 0;
    int test;
    // this solution is faster but has 1024 chars/line limitation
    do {
        cur_size = 0;
        while ((test = read(inp, &cur_ch, sizeof(char)))) {
            // no need to empty buffer, it will be overwritten
            buf[cur_size++] = cur_ch;
            if (cur_ch == '\n') break;
        }
        if (line_ctr < 100) {
            write(f1, buf, cur_size);
            ++line_ctr;
        } else {
            write(f2, buf, cur_size);
        }
    } while (test == 1);

    // // this solution is very slow but easiest to implement and has no 1024 chars/line limitation
    // while (read(inp, &cur_ch, sizeof(char))) {
    //     if (line_ctr < 100)
    //         write(f1, &cur_ch, sizeof(char));
    //     else
    //         write(f2, &cur_ch, sizeof(char));
    //     if (cur_ch == '\n') ++line_ctr;
    // }
    close(inp);
    close(f1);
    close(f2);
}