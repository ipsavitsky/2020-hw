#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

graph upload_graph(char *filename) {
    FILE *input = fopen(filename, "r");
    graph_node *new_arr = NULL;
    size_t struct_size;
    if (input == NULL) {
        perror("upload_struct(): ");
        exit(1);
    }
    fread(&struct_size, sizeof(size_t), 1, input);
    new_arr = malloc(struct_size * sizeof(graph_node));
    fread(new_arr, sizeof(graph_node), struct_size, input);
    size_t arr_size;
    for (int i = 0; i < struct_size; ++i) {
        switch (new_arr[i].data_type) {
            case INTEGER:
                arr_size = sizeof(int);
                break;

            case DOUBLE:
                arr_size = sizeof(double);
                break;

            case CHAR:
                arr_size = sizeof(char);
                break;
        }
        new_arr[i].data = malloc(new_arr[i].data_size * arr_size);
        fread(new_arr[i].data, arr_size, new_arr[i].data_size, input);
    }
    int *relative_addresses = NULL;
    for (int i = 0; i < struct_size; ++i) {
        new_arr[i].edges = malloc(new_arr[i].num_edges * sizeof(graph_node *));
        relative_addresses =
            realloc(relative_addresses, new_arr[i].num_edges * sizeof(int));
        fread(relative_addresses, sizeof(int), new_arr[i].num_edges, input);
        for (int j = 0; j < new_arr[i].num_edges; ++j)
            new_arr[i].edges[j] = new_arr + relative_addresses[j];
    }
    free(relative_addresses);
    graph out;
    out.vertices = new_arr;
    out.num_vertices = struct_size;
    fclose(input);
    return out;
}

void save_graph(char *filename, graph sv_graph) {
    FILE *output = fopen(filename, "w");
    fwrite(&(sv_graph.num_vertices), sizeof(sv_graph.num_vertices), 1, output);
    fwrite(sv_graph.vertices, sizeof(graph_node), sv_graph.num_vertices,
           output);
    size_t arr_size;
    for (int i = 0; i < sv_graph.num_vertices; ++i) {
        switch (sv_graph.vertices[i].data_type) {
            case INTEGER:
                arr_size = sizeof(int);
                break;

            case DOUBLE:
                arr_size = sizeof(double);
                break;

            case CHAR:
                arr_size = sizeof(char);
                break;
        }
        fwrite(sv_graph.vertices[i].data, arr_size,
               sv_graph.vertices[i].data_size, output);
    }
    int relative_address;
    for (int i = 0; i < sv_graph.num_vertices; ++i) {
        for (int j = 0; j < sv_graph.vertices[i].num_edges; ++j) {
            relative_address =
                sv_graph.vertices[i].edges[j] - sv_graph.vertices;
            fwrite(&relative_address, sizeof(int), 1, output);
        }
    }
    fclose(output);
}

void print_graph(graph in_graph) {
    for (int i = 0; i < in_graph.num_vertices; ++i) {
        printf("================vertex [%d]================\n", i);
        printf("data(%zu):\n", in_graph.vertices[i].data_size);
        for (int j = 0; j < in_graph.vertices[i].data_size; ++j) {
            switch (in_graph.vertices[i].data_type) {
                case INTEGER:
                    printf("%d ", *((int *)in_graph.vertices[i].data + j));
                    break;
                case DOUBLE:
                    printf("%lf ", *((double *)in_graph.vertices[i].data + j));
                    break;
                case CHAR:
                    printf("%c", *((char *)in_graph.vertices[i].data + j));
                default:
                    break;
            }
        }
        printf("\n");
        int relative_address;
        printf("has edges to vertces(%zu):\n", in_graph.vertices[i].num_edges);
        for (int j = 0; j < in_graph.vertices[i].num_edges; ++j) {
            relative_address =
                in_graph.vertices[i].edges[j] - in_graph.vertices;
            printf("%d ", relative_address);
        }
        printf("\n");
    }
    // printf("\n");
}

void add_vertex(graph *in_graph, enum Types data_type, void *data,
                size_t data_size) {
    in_graph->vertices = realloc(
        in_graph->vertices, (++(in_graph->num_vertices)) * sizeof(graph_node));
    in_graph->vertices[in_graph->num_vertices - 1].data_size = data_size;
    in_graph->vertices[in_graph->num_vertices - 1].data_type = data_type;
    in_graph->vertices[in_graph->num_vertices - 1].num_edges = 0;
    in_graph->vertices[in_graph->num_vertices - 1].edges = NULL;
    size_t arr_size;
    switch (data_type) {
        case INTEGER:
            arr_size = sizeof(int);
            break;

        case DOUBLE:
            arr_size = sizeof(double);
            break;

        case CHAR:
            arr_size = sizeof(char);
            break;
    }
    in_graph->vertices[in_graph->num_vertices - 1].data =
        malloc(data_size * arr_size);
    for (int i = 0; i < data_size; ++i) {
        switch (data_type) {
            case INTEGER:
                ((int *)(in_graph->vertices[in_graph->num_vertices - 1]
                             .data))[i] = ((int *)data)[i];
                break;
            case DOUBLE:
                ((double *)(in_graph->vertices[in_graph->num_vertices - 1]
                                .data))[i] = ((double *)data)[i];
                break;
            case CHAR:
                ((char *)(in_graph->vertices[in_graph->num_vertices - 1]
                              .data))[i] = ((char *)data)[i];
                break;
        }
    }
}

void delete_graph(graph *del_graph) {
    for (int i = 0; i < del_graph->num_vertices; ++i) {
        free(del_graph->vertices[i].data);
        free(del_graph->vertices[i].edges);
    }
    free(del_graph->vertices);
}

void add_edge(graph *graph, int offset1, int offset2) {
    graph->vertices[offset1].edges = realloc(
        graph->vertices[offset1].edges,
        (++(graph->vertices[offset1].num_edges)) * sizeof(graph_node *));
    graph->vertices[offset1].edges[graph->vertices[offset1].num_edges - 1] = graph->vertices + offset2;
}

void grapviz_output(graph pr_graph, char *filename){
    FILE *output = fopen(filename, "w");
    fprintf(output, "digraph\n{\n");
    for(int i = 0; i < pr_graph.num_vertices; ++i){
        
        graph_node *cur = NULL;
        for(int j = 0; j < pr_graph.vertices[i].num_edges; ++j){
            fprintf(output, "\"");
            for (int k = 0; k < pr_graph.vertices[i].data_size; ++k) {
                switch (pr_graph.vertices[i].data_type) {
                    case INTEGER:
                        fprintf(output, "%d ", *((int *)pr_graph.vertices[i].data + k));
                        break;

                    case DOUBLE:
                        fprintf(output, "%lf ", *((double *)pr_graph.vertices[i].data + k));
                        break;

                    case CHAR:
                        fprintf(output, "%c", *((char *)pr_graph.vertices[i].data + k));
                        break;
                }
            }
            fprintf(output, "\"->\"");
            // fprintf(output, "\"");
            cur = pr_graph.vertices[i].edges[j];
            for(int k = 0; k < cur->data_size; ++k){
                switch (cur->data_type) {
                    case INTEGER:
                        fprintf(output, "%d ", *((int *)(cur->data) + k));
                        break;

                    case DOUBLE:
                        fprintf(output, "%lf ", *((double *)(cur->data) + k));
                        break;

                    case CHAR:
                        fprintf(output, "%c", *((char *)(cur->data) + k));
                        break;
                }
            }
            fprintf(output, "\" \n");
        }
        // fprintf(output, "\n");
    }
    fprintf(output, "}");
}