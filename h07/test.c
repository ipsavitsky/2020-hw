#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *f;
    long pos, n;
    int *mas = calloc(8, sizeof(int));
    mas[0] = 1;
    mas[1] = 2;
    mas[2] = 3;
    mas[3] = 4;
    mas[4] = 5;
    mas[5] = 6;
    mas[6] = 7;
    mas[7] = 8;

    f = fopen("input", "w");
    assert(f != NULL);
    fwrite(mas, sizeof(int), 8, f);
    fclose(f);
    free(mas);
}