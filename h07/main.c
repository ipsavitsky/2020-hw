#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// specification1: it is guaranteed that the binary "input" file exists
// specification2: the size of "input" file is less than long int(in bytes)

void reverse(FILE* fr) {
    long int strt = 0, end = -sizeof(int), size;
    int tmp1 = 0, tmp2 = 0;
    fseek(fr, 0, SEEK_END);
    size = ftell(fr);
    size /= sizeof(int);
    for (int i = 0; i < size / 2; ++i) {
        fseek(fr, strt, SEEK_SET);
        fread(&tmp1, sizeof(int), 1, fr);
        fseek(fr, end, SEEK_END);
        fread(&tmp2, sizeof(int), 1, fr);
        fseek(fr, -sizeof(int), SEEK_CUR);
        fwrite(&tmp1, sizeof(int), 1, fr);
        fseek(fr, strt, SEEK_SET);
        fwrite(&tmp2, sizeof(int), 1, fr);
        strt += sizeof(int);
        end -= sizeof(int);
    }
}

int main(void) {
    FILE* f;
    f = fopen("input", "r+");
    assert(f != NULL);
    reverse(f);
    fclose(f);
}