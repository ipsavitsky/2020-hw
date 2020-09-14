#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graphs.h"

/**
 * \file
 * \brief graph implementation
 */

struct Graph upload_graph(char *filename)
{
    FILE *filein;
    int i, flag, j;
    struct Graph result;
    filein = fopen(filename, "r");
    assert(filein != NULL);
    fscanf(filein, "%d", &(result.vernum));
    result.vertices = malloc(result.vernum * sizeof(struct Vertex));
    assert(result.vertices != NULL);
    for (i = 0; i < result.vernum; i++)
    {
        for (j = 0; j < result.vernum; j++)
        {
            fscanf(filein, "%d", &flag);
            if (flag == 1)
                add_edge(&result, i, j);
        }
    }
    fclose(filein);
    return result;
}

void add_edge(struct Graph *graph, int from, int to)
{
    assert(from >= 0);
    assert(to >= 0);
    assert(graph != NULL);
    assert(from < graph->vernum);
    assert(to < graph->vernum);
    struct Vertex *cur = graph->vertices[from];
    if (cur == NULL)
    {
        graph->vertices[from] = malloc(sizeof(struct Vertex));
        assert(graph->vertices[from] != NULL);
        graph->vertices[from]->next = NULL;
        graph->vertices[from]->number = to;
        //graph -> vertexes[from] -> weight = weight;
        return;
    }
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = malloc(sizeof(struct Vertex));
    assert(cur->next != NULL);
    cur->next->next = NULL;
    cur->next->number = to;
    return;
}

void print_graph(struct Graph graph)
{
    int i;
    struct Vertex *safe;
    for (i = 0; i < graph.vernum; i++)
    {
        printf("[%d]: ", i);
        safe = graph.vertices[i];
        if (safe == NULL)
        {
            printf("NULL\n");
            continue;
        }
        while (safe != NULL)
        {
            printf("%d; ", safe->number);
            safe = safe->next;
        }
        printf("\n");
    }
}

void delete_graph(struct Graph *graph){
    int i;
    struct Vertex *safe, *prev;
    for (i = 0; i < graph->vernum; i++)
    {
        prev = graph->vertices[i];
        if(prev == NULL)
            continue;
        while (prev != NULL)
        {
            safe = prev->next;
            free(prev);
            prev = safe;
        }
    }
    free(graph -> vertices);
}

void add_vertex(struct Graph *graph){
    assert(graph != NULL);
    graph->vernum += 1;
    graph->vertices = realloc(graph->vertices, graph->vernum * sizeof(struct Graph));
    assert(graph->vertices != NULL);
    // printGraph(*graph);
    graph->vertices[graph->vernum - 1] = NULL;
}

void remove_vertex(struct Graph *graph, int vertex){
    struct Vertex *prev, *cur;
    int i;
    assert(vertex >= 0);
    assert(graph != NULL);
    assert(vertex < graph->vernum);
    prev = graph->vertices[vertex];
    if(prev != NULL){
        while (prev != NULL)
        {
            cur = prev->next;
            free(prev);
            prev = cur;
        }
    }
    for(i = vertex+1; i < graph -> vernum; i++)
        graph -> vertices[i - 1] = graph -> vertices[i];
    graph -> vernum -= 1;
    for(i = 0; i < graph -> vernum; i++){
        cur = graph -> vertices[i];
        prev = graph -> vertices[i];
        while(cur != NULL){
            if(cur -> number == vertex){
                if(prev == cur){
                    cur = cur -> next;
                    free(prev);
                    prev = cur;
                    graph -> vertices[i] = cur;
                    continue;
                }
                prev -> next = cur -> next;
                free(cur);
                cur = prev;
            }
            else if(cur -> number > vertex){
                cur -> number -= 1;
            }
            prev = cur;
            cur = cur -> next;
        }
    }
}

void remove_edge(struct Graph *graph, int from, int to){
    assert(from >= 0);
    assert(to >= 0);
    assert(graph != NULL);
    assert(from < graph->vernum);
    assert(to < graph->vernum);
    struct Vertex *cur = graph->vertices[from];
    struct Vertex *prev = graph->vertices[from];
    while (cur != NULL)
    {
        if (cur->number == to)
        {
            if (prev == cur)
            {
                cur = cur->next;
                free(prev);
                prev = cur;
                graph->vertices[from] = cur;
                continue;
            }
            prev->next = cur->next;
            free(cur);
            cur = prev;
        }
        prev = cur;
        cur = cur->next;
    }
}

int way_count(struct Graph graph, int from, int to, int *control)
{
    assert(from < graph.vernum);
    assert(to < graph.vernum);
    assert(from >= 0);
    assert(to >= 0);
    assert(control != NULL);
    struct Vertex *cur = malloc(sizeof(struct Vertex));
    assert(cur != NULL);
    int *ncontrol = malloc(sizeof(int) * graph.vernum);
    assert(ncontrol != NULL);
    int sum = 0, i;
    for (i = 0; i < graph.vernum; i++)
        ncontrol[i] = control[i];
    if (from == to)
        return 1;
    cur = graph.vertices[from];
    ncontrol[from] = 1;
    while (cur != NULL)
    {
        if (ncontrol[cur->number] == 0)
            sum += way_count(graph, cur->number, to, ncontrol);
        cur = cur->next;
    }
    free(cur);
    free(ncontrol);
    return sum;
}