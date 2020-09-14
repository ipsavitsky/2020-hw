#include "graphs.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    struct Graph graph;
    graph = upload_graph("test.txt");
    print_graph(graph);
    int from, to;
    int *cont = calloc(graph.vernum, sizeof(int));
    scanf("%d%d", &from, &to);
    printf("there are %d ways from %d to %d\n", way_count(graph, from, to, cont), from, to);
    free(cont);
    printf("reverting\n");
    struct Graph rev_graph = revert_graph(graph);
    print_graphviz(rev_graph);
    delete_graph(&graph);
    delete_graph(&rev_graph);
}
