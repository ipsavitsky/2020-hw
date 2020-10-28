#include "graph.h"
#include <string.h>

int main(void){
    graph test_graph;
    test_graph.vertices = NULL;
    test_graph.num_vertices = 0;
    int test[] = {5, 4, 3, 2, 1};
    double test_d[] = {1., 1.5, 2., 2.5, 3.};
    add_vertex(&test_graph, CHAR, "abcd", 4);
    add_vertex(&test_graph, INTEGER, test, 5);
    add_vertex(&test_graph, DOUBLE, test_d, 5);
    add_edge(&(test_graph.vertices[1]), &(test_graph.vertices[2]));
    add_edge(&(test_graph.vertices[2]), &(test_graph.vertices[2]));
    add_edge(&(test_graph.vertices[0]), &(test_graph.vertices[1]));
    add_edge(&(test_graph.vertices[0]), &(test_graph.vertices[1]));
    add_edge(&(test_graph.vertices[0]), &(test_graph.vertices[1]));
    add_edge(&(test_graph.vertices[1]), &(test_graph.vertices[0]));
    add_edge(&(test_graph.vertices[1]), &(test_graph.vertices[2]));
    add_edge(&(test_graph.vertices[2]), &(test_graph.vertices[0]));
    delete_edge(&(test_graph.vertices[0]), &(test_graph.vertices[1]));
    delete_vertex(&(test_graph), &(test_graph.vertices[2]));
    // delete_edge(&(test_graph.vertices[2]), &(test_graph.vertices[2]));
    print_graph(test_graph);
    grapviz_output(test_graph, "out.dot");
    // save_graph("test.gr", test_graph);
    delete_graph(&test_graph);
    // graph new_gr = upload_graph("graph.gr");
    // print_graph(new_gr);
    // delete_graph(&new_gr);
}