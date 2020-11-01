#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"

void quicksort_int(int *A, int len) {
    if (len < 2) return;

    int pivot = A[len / 2];

    int i, j;
    for (i = 0, j = len - 1;; i++, j--) {
        while (A[i] < pivot) i++;
        while (A[j] > pivot) j--;

        if (i >= j) break;

        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }

    quicksort_int(A, i);
    quicksort_int(A + i, len - i);
}

void quicksort_char(char *A, int len) {
    if (len < 2) return;

    char pivot = A[len / 2];

    int i, j;
    for (i = 0, j = len - 1;; i++, j--) {
        while (A[i] < pivot) i++;
        while (A[j] > pivot) j--;

        if (i >= j) break;

        char temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }

    quicksort_char(A, i);
    quicksort_char(A + i, len - i);
}

void quicksort_double(double *A, int len) {
    if (len < 2) return;

    double pivot = A[len / 2];

    int i, j;
    for (i = 0, j = len - 1;; i++, j--) {
        while (A[i] < pivot) i++;
        while (A[j] > pivot) j--;

        if (i >= j) break;

        double temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }

    quicksort_double(A, i);
    quicksort_double(A + i, len - i);
}

void quicksort(void *A, size_t len, enum Types data_type){
    switch (data_type)
    {
    case INTEGER:
        quicksort_int((int *)A, len);
        break;
    case DOUBLE:
        quicksort_double((double *)A, len);
        break;
    case CHAR:
        quicksort_char((char *)A, len);
        break;
    default:
        break;
    }
}

int main(void) {
    graph test_graph;
    test_graph.vertices = NULL;
    test_graph.num_vertices = 0;
    int test1[] = {5, 4, 3, 2, 1};
    double test2[] = {5., 4.5, 3., 2.5, 1.};
    add_vertex(&test_graph, INTEGER, &test1, 5);
    add_vertex(&test_graph, DOUBLE, &test2, 5);
    add_vertex(&test_graph, CHAR, "what?", 5);
    delete_vertex(&test_graph, &(test_graph.vertices[0]));
    print_graph(test_graph);
    traverse(&test_graph, quicksort);
    print_graph(test_graph);
    delete_graph(&test_graph);
}