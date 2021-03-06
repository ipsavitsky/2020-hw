#include <assert.h>
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"


// квиксорты тут для демонстрации
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
    // создаем граф
    graph test_graph;
    test_graph.vertices = NULL;
    test_graph.num_vertices = 0;
    int test1[] = {5, 4, 3, 2, 1};
    double test2[] = {2.5, 1., 3., 4.5, 10};


    // добавляем вершины, получается такой граф
    /*
        0+--2
        +  /+
        | / |
        |+  |
        1+--3
    */
    // где номер 1-2-3-4 соответствует порядку добавления
    add_vertex(&test_graph, CHAR, "Ayli", 4);
    add_vertex(&test_graph, INTEGER, test1, 5);
    add_vertex(&test_graph, CHAR, "ykstivas", 8);
    add_vertex(&test_graph, DOUBLE, test2, 5);
    graph_node **vertices = NULL;
    // мэтчим указатели для удобной работы с добавлением(да, костыль)
    vertices = match_vertices(vertices, test_graph);
    add_edge(vertices[1], vertices[0]);
    add_edge(vertices[2], vertices[0]);
    add_edge(vertices[2], vertices[1]);
    add_edge(vertices[3], vertices[1]);
    add_edge(vertices[3], vertices[2]);

    // выводим граф
    print_graph(test_graph);

    // Удаляем вершину 2 и снова мэтчим вершины
    /*
        0
        +
        |
        |
        1+--3
    */
    delete_vertex(&test_graph, vertices[2]);
    vertices = match_vertices(vertices, test_graph);

    // добавляем 3 ребра из 0 в 3(2 - новый замэтченный 3, match_vertices учитывает удаленные вершины)
    add_edge(vertices[0], vertices[2]);
    add_edge(vertices[0], vertices[2]);
    add_edge(vertices[0], vertices[2]);

    // добавляем вершину(она добавится в дырку от удаленной старой вершины) и снова мэтчим вершины
    // но новая вершина встанет на второе место(как это пофиксить я пока не придумал)
    add_vertex(&test_graph, CHAR, "what is this?", 13);
    vertices = match_vertices(vertices, test_graph);

    /*
        0
        +\
        | \
        |  +(х3)
        1+--3
        +
        |
        +
        2
    */
    add_edge(vertices[2], vertices[1]);
    add_edge(vertices[1], vertices[2]);

    // удалит кратные ребра(из 0 в 3 останется одно)
    reduce_connections(&test_graph);

    // сохраняем граф в файл и удаляем его, загружаем новый граф из того же файла и мэтчим его вершины
    save_graph("graph.gr", test_graph);
    delete_graph(&test_graph);
    graph new_graph = upload_graph("graph.gr");
    vertices = match_vertices(vertices, new_graph);

    // удаляем вторую вершину и обходим граф, сортируя функцией quicksort, объявленной в этом файле!
    delete_vertex(&new_graph, vertices[2]);
    traverse(&new_graph, quicksort);
    print_graph(new_graph);
    // записываем оставшийся граф в файл в формате graphviz и удаляем его
    graphviz_output(new_graph, "graph.dot");
    delete_graph(&new_graph);
    free(vertices);
}