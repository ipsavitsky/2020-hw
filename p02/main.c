#include "graphs.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    struct Graph graph;
    graph = uploadGraph("test.txt");
    printGraph(graph);
    delete_graph(&graph);
}
