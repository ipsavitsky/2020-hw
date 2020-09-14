#include "graphs.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    struct Graph graph;
    graph = upload_graph("test.txt");
    print_graph(graph);
    struct Path *path = malloc(sizeof(struct Path));
    path->length = 1;
    path->path = malloc(sizeof(int));
    // assert(path->path != NULL);
    print_all_paths(graph, 0, 2, path);
    free(path -> path);
    free(path);
    delete_graph(&graph);
}
