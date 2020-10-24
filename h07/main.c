#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// parameters for testing
#define MAX_TEST 300 /* amount of tests to generate */
#define FILENAME_SIZE 32 /* filename size; do not change unless you plan something ludicrous */
#define TEST_SIZE 10000 /* maximum size of each test */

// specification1: if tou plan to run tests, ensure that directory tests exists
// specification2: the size of  file is less than long int(in bytes)

/**
 * \brief reverse the input file
 * \param fi already opened file that needs to be
 */
void freverse(FILE *fi) {
    assert(fi != NULL);
    long int strt = 0, end = -sizeof(int), size, ptr;
    int tmp1 = 0, tmp2 = 0;
    ptr = ftell(fi);
    fseek(fi, 0, SEEK_END);
    size = ftell(fi);
    size /= sizeof(int);
    for (int i = 0; i < size / 2; ++i) {
        fseek(fi, strt, SEEK_SET);
        fread(&tmp1, sizeof(int), 1, fi);
        fseek(fi, end, SEEK_END);
        fread(&tmp2, sizeof(int), 1, fi);
        fseek(fi, -sizeof(int), SEEK_CUR);
        fwrite(&tmp1, sizeof(int), 1, fi);
        fseek(fi, strt, SEEK_SET);
        fwrite(&tmp2, sizeof(int), 1, fi);
        strt += sizeof(int);
        end -= sizeof(int);
    }
    // flush in case we need to read from this file again
    fflush(fi);
    // preserve the original pointer state
    fseek(fi, ptr, SEEK_SET);
}

int main(int argc, char *argv[]) {
    // if we input a file - reverse it
    if (argc == 2) {
        FILE *f;
        f = fopen(argv[1], "r+");
        assert(f != NULL);
        freverse(f);
        fclose(f);
        return 0;
    }
    // if we input nothing - generate tests and run them(kind of like a fuzzer
    // but random and checks answer)
    else if (argc == 1) {
        char *filename = NULL;
        int am, *test_arr;
        FILE *cur_file;
        printf("generating tests\n");
        srand(time(NULL));
        for (int i = 0; i < MAX_TEST; ++i) {
            // reserve memory for filename
            filename = calloc(FILENAME_SIZE, sizeof(char));
            assert(filename != NULL);
            // generate filename
            sprintf(filename, "./tests/test-%d", i);
            cur_file = fopen(filename, "w");
            assert(cur_file != NULL);
            // generate file's size
            am = rand() % TEST_SIZE;
            // file_data(saved in an array to check it later)
            test_arr = calloc(am, sizeof(test_arr));
            assert(test_arr != NULL);
            // generate file data
            for (int j = 0; j < am; ++j) test_arr[j] = rand() % __INT_MAX__;
            fwrite(test_arr, sizeof(int), am, cur_file);
            fclose(cur_file);
            // reverse file and test it with initial data
            cur_file = fopen(filename, "r+");
            assert(cur_file != NULL);
            freverse(cur_file);
            fseek(cur_file, 0L, SEEK_SET);
            int tmp;
            for (int k = 0; k < am; ++k) {
                fread(&tmp, sizeof(int), 1, cur_file);
                if (tmp != test_arr[am - k - 1]) {
                    printf("%d: FAILED on %d(%d != %d)\n", i, k, tmp, test_arr[am - k - 1]);
                    fclose(cur_file);
                    free(filename);
                    free(test_arr);
                    return 1;
                }
            }
            fclose(cur_file);
            printf("%d: OK\t(%d)\n", i, am);
            free(filename);
            free(test_arr);
        }
        return 0;
    } else {
        // somethin weird happaned
        printf("Invalid arguments!\n");
        return 0;
    }
}