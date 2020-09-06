#include "graphs.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    struct Graph graph;
    graph = uploadGraph("test.txt");
    printGraph(graph);
    add_vertex(&graph);
    addEdge(&graph, 4, 2);
    printGraph(graph);
    delete_graph(&graph);
}
