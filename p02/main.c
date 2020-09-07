#include "graphs.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    struct Graph graph;
    graph = upload_graph("test.txt");
    print_graph(graph);
    int* control = calloc(graph.vernum, sizeof(int));
    printf("%d", way_count_node(graph, 0, 2, control));
    // print_graph(graph);
    delete_graph(&graph);
}