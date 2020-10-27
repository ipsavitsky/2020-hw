#include <stddef.h>

enum Types {
    INTEGER,
    DOUBLE,
    CHAR

};

typedef struct Graph_node{
    void *data;
    size_t data_size;
    enum Types data_type;
    struct Graph_node **edges;
    size_t num_edges;
} graph_node;

typedef struct{
    size_t num_vertices;
    graph_node *vertices;  
} graph;



graph upload_graph(char* filename);
void save_graph(char *filename, graph sv_graph);
void print_graph(graph in_graph);
void add_vertex(graph *in_graph, enum Types data_type, void *data, size_t data_size);
void add_edge(graph *graph, int offset1, int offset2);
void grapviz_output(graph pr_graph, char *filename);
void delete_graph(graph *del_graph);