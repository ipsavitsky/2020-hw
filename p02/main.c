#include "graphs.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    struct Graph graph;
    graph = upload_graph("test.txt");
    print_graph(graph);
    printf("reverting\n");
    graph = revert_graph(graph);
    print_graph(graph);
    delete_graph(&graph);
}
