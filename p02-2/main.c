#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void){
    graph test_graph;
    test_graph.vertices = NULL;
    test_graph.num_vertices = 0;
    int test[] = {5, 4, 3, 2, 1};
    double test_d[] = {1., 1.5, 2., 2.5, 3.};
    graph_node *what[4];
    what[0] = add_vertex(&test_graph, CHAR, "abcd", 4);
    what[1] = add_vertex(&test_graph, INTEGER, test, 5);
    what[2] = add_vertex(&test_graph, DOUBLE, test_d, 5);
    // assert(what[0] != NULL);
    // assert(what[0] == &(test_graph.vertices[0]));
    // add_edge(what[0], what[1]);
    // add_edge(what[1], what[2]);
    // add_edge(what[2], what[0]);
    add_edge(&(test_graph.vertices[0]), &(test_graph.vertices[1]));
    add_edge(&(test_graph.vertices[1]), &(test_graph.vertices[2]));
    add_edge(&(test_graph.vertices[2]), &(test_graph.vertices[0]));
    // print_graph(test_graph);
    delete_vertex(&test_graph, &(test_graph.vertices[2]));
    what[3] = add_vertex(&test_graph, CHAR, "what?", 5);
    print_graph(test_graph);
    delete_graph(&test_graph);
}