#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

graph upload_graph(char *filename) {
    FILE *input = fopen(filename, "r");
    graph_node *new_arr = NULL;
    size_t struct_size;
    if (input == NULL) {
        perror("upload_struct(): ");
        exit(1);
    }
    if(fread(&struct_size, sizeof(size_t), 1, input) == 0) exit(1);
    new_arr = malloc(struct_size * sizeof(graph_node));
    if(fread(new_arr, sizeof(graph_node), struct_size, input)) exit(1);
    size_t arr_size = 0;
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
            default:
                break;
        }
        new_arr[i].data = malloc(new_arr[i].data_size * arr_size);
        if(fread(new_arr[i].data, arr_size, new_arr[i].data_size, input)) exit(1);
    }
    int *relative_addresses = NULL;
    for (int i = 0; i < struct_size; ++i) {
        new_arr[i].edges = malloc(new_arr[i].num_edges * sizeof(graph_node *));
        relative_addresses =
            realloc(relative_addresses, new_arr[i].num_edges * sizeof(int));
        if(fread(relative_addresses, sizeof(int), new_arr[i].num_edges, input)) exit(1);
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
    size_t arr_size = 0;
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
            default:
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
    int ctr = 0;
    for (int i = 0; i < in_graph.num_vertices; ++i) {
        if (in_graph.vertices[i].existent) {
            printf("================vertex[%d]================\n", i);
            printf("data(%zu):\n", in_graph.vertices[i].data_size);
            for (int j = 0; j < in_graph.vertices[i].data_size; ++j) {
                switch (in_graph.vertices[i].data_type) {
                    case INTEGER:
                        printf("%d ", *((int *)in_graph.vertices[i].data + j));
                        break;
                    case DOUBLE:
                        printf("%lf ",
                               *((double *)in_graph.vertices[i].data + j));
                        break;
                    case CHAR:
                        printf("%c", *((char *)in_graph.vertices[i].data + j));
                        break;
                    default:
                        break;
                }
            }
            printf("\n");
            int relative_address;
            printf("has edges to vertices(%zu):\n",
                   in_graph.vertices[i].num_edges);
            for (int j = 0; j < in_graph.vertices[i].num_edges; ++j) {
                if (in_graph.vertices[i].edges[j]->existent) {
                    relative_address =
                        in_graph.vertices[i].edges[j] - in_graph.vertices;
                    printf("%d ", relative_address);
                }
            }
            printf("\n");
            ++ctr;
        }
    }
    printf("=========================================\n");
    printf("there are also %lu non-existent vertices\n",
           in_graph.num_vertices - ctr);
}

graph_node *add_vertex(graph *in_graph, enum Types data_type, void *data,
                       size_t data_size) {
    int pos = 0;
    for (pos = 0; pos < in_graph->num_vertices; pos++)
        if (in_graph->vertices[pos].existent == 0) break;
    if (pos == in_graph->num_vertices) {
        int **relative = malloc(sizeof(int *) * in_graph->num_vertices);
        for (int i = 0; i < in_graph->num_vertices; ++i) {
            relative[i] = malloc(sizeof(int) * in_graph->vertices[i].num_edges);
            for (int j = 0; j < in_graph->vertices[i].num_edges; ++j) {
                relative[i][j] =
                    in_graph->vertices[i].edges[j] - in_graph->vertices;
                // printf("%d ", relative[i][j]);
            }
            // printf("\n");
            free(in_graph->vertices[i].edges);
        }
        in_graph->vertices =
            realloc(in_graph->vertices,
                    (++(in_graph->num_vertices)) * sizeof(graph_node));
        for (int i = 0; i < in_graph->num_vertices - 1; ++i) {
            in_graph->vertices[i].edges =
                malloc(sizeof(graph_node *) * in_graph->vertices[i].num_edges);
            for (int j = 0; j < in_graph->vertices[i].num_edges; ++j) {
                in_graph->vertices[i].edges[j] =
                    relative[i][j] + in_graph->vertices;
            }
            free(relative[i]);
        }
        free(relative);
        pos = in_graph->num_vertices - 1;
    }
    // printf("pos: %d\n", pos);
    in_graph->vertices[pos].data_type = data_type;
    in_graph->vertices[pos].data_size = data_size;
    in_graph->vertices[pos].num_edges = 0;
    in_graph->vertices[pos].edges = 0;
    in_graph->vertices[pos].existent = 1;
    size_t arr_size = 0;
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
        default:
            break;
    }
    in_graph->vertices[pos].data = malloc(data_size * arr_size);

    memmove(in_graph->vertices[pos].data, data, data_size * arr_size);

    return &(in_graph->vertices[pos]);
}

void delete_graph(graph *del_graph) {
    for (int i = 0; i < del_graph->num_vertices; ++i) {
        if (del_graph->vertices[i].existent != 0) {
            free(del_graph->vertices[i].data);
            free(del_graph->vertices[i].edges);
        }
    }
    free(del_graph->vertices);
}

void add_edge(graph_node *node1, graph_node *node2) {
    node1->edges =
        realloc(node1->edges, (++(node1->num_edges)) * sizeof(graph_node *));
    node1->edges[node1->num_edges - 1] = node2;
}

void grapviz_output(graph pr_graph, char *filename) {
    FILE *output = fopen(filename, "w");
    fprintf(output, "digraph\n{\n");
    for (int i = 0; i < pr_graph.num_vertices; ++i) {
        graph_node *cur = NULL;
        if (pr_graph.vertices[i].existent != 0) {
            for (int j = 0; j < pr_graph.vertices[i].num_edges; ++j) {
                fprintf(output, "\"");
                for (int k = 0; k < pr_graph.vertices[i].data_size; ++k) {
                    switch (pr_graph.vertices[i].data_type) {
                        case INTEGER:
                            fprintf(output, "%d ",
                                    *((int *)pr_graph.vertices[i].data + k));
                            break;

                        case DOUBLE:
                            fprintf(output, "%lf ",
                                    *((double *)pr_graph.vertices[i].data + k));
                            break;

                        case CHAR:
                            fprintf(output, "%c",
                                    *((char *)pr_graph.vertices[i].data + k));
                            break;
                        default:
                            break;
                    }
                }
                fprintf(output, "\"->\"");
                // fprintf(output, "\"");
                cur = pr_graph.vertices[i].edges[j];
                for (int k = 0; k < cur->data_size; ++k) {
                    switch (cur->data_type) {
                        case INTEGER:
                            fprintf(output, "%d ", *((int *)(cur->data) + k));
                            break;

                        case DOUBLE:
                            fprintf(output, "%lf ",
                                    *((double *)(cur->data) + k));
                            break;

                        case CHAR:
                            fprintf(output, "%c", *((char *)(cur->data) + k));
                            break;
                        default:
                            break;
                    }
                }
                fprintf(output, "\" \n");
            }
        }
    }

    fprintf(output, "}");
    fclose(output);
}

void delete_edge(graph_node *node1, graph_node *node2) {
    for (int i = 0; i < node1->num_edges; ++i) {
        if (node1->edges[i] == node2) {
            for (int j = i; j < node1->num_edges - 1; ++j) {
                node1->edges[j] = node1->edges[j + 1];
            }
            node1->edges = realloc(
                node1->edges, (--(node1->num_edges)) * sizeof(graph_node *));
            return;
        }
    }
}

void delete_vertex(graph *gr, graph_node *node) {
    node->existent = 0;
    for (int i = 0; i < gr->num_vertices; ++i) {
        for (int j = 0; j < gr->vertices[i].num_edges; ++j) {
            if (gr->vertices[i].edges[j] == node) {
                delete_edge(&(gr->vertices[i]), node);
            }
        }
    }
    free(node->data);
    free(node->edges);
}

void reduce_connections(graph *gr) {
    for (int i = 0; i < gr->num_vertices; ++i) {
        if (gr->vertices[i].num_edges == 0) continue;
        printf("gr->vertices[%d].num_edges = %zu\n", i,
               gr->vertices[i].num_edges);
        for (int j = 0; j < gr->vertices[i].num_edges; ++j) {
            printf("checking out %d\n", j);
            for (int k = j + 1; k < gr->vertices[i].num_edges; ++k) {
                if (gr->vertices[i].edges[j] == gr->vertices[i].edges[k]) {
                    // printf("found %d\n", i);
                    delete_edge(&(gr->vertices[i]), gr->vertices[i].edges[j]);
                }
                // printf("deleted between %d and %d/%zu\n", j, k,
                // gr->vertices[i].num_edges);
            }
        }
    }
}

void traverse(graph *gr, data_func function) {
    for (int i = 0; i < gr->num_vertices; ++i)
        if (gr->vertices[i].existent != 0)
            function(gr->vertices[i].data, gr->vertices[i].data_size,
                     gr->vertices[i].data_type);
}
