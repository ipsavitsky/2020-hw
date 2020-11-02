#include "graphs.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){ 
    struct Graph graph;
    graph = upload_graph("test.txt");
    add_vertex(&graph);
    add_edge(&graph, 1, 4);
    print_graph(graph);
    int from, to;
    int *cont = calloc(graph.vernum, sizeof(int));
    scanf("%d%d", &from, &to);
    printf("there are %d ways from %d to %d\n", way_count(graph, from, to, cont), from, to);
    free(cont);
    remove_vertex(&graph, 1);
    // print_graph(graph);
    remove_edge(&graph, 1, 0);
    printf("reverting\n");
    struct Graph rev_graph = revert_graph(graph);
    print_graphviz(rev_graph);
    delete_graph(&graph);
    delete_graph(&rev_graph);
}