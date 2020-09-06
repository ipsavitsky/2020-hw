#include "graphs.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    struct Graph graph;
    graph = uploadGraph("test.txt");
    printGraph(graph);
    remove_vertex(&graph, 2);
    printGraph(graph);
    delete_graph(&graph);
}
