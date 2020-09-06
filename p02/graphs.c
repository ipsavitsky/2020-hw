#include <stdio.h>
#include <stdlib.h>
#include "graphs.h"

/**
 * \brief initialize graph from file
 * \param filename name of the file from which to upload
 * \return graph which the matrix determines
 */
struct Graph uploadGraph(char *filename)
{
    FILE *filein;
    int i, flag, j;
    struct Graph result;
    filein = fopen(filename, "r");
    fscanf(filein, "%d", &(result.vernum));
    result.vertices = malloc(result.vernum * sizeof(struct Vertex));
    for (i = 0; i < result.vernum; i++)
    {
        for (j = 0; j < result.vernum; j++)
        {
            fscanf(filein, "%d", &flag);
            if (flag == 1)
                addEdge(&result, i, j);
        }
    }
    fclose(filein);
    return result;
}

void addEdge(struct Graph *graph, int from, int to)
{
    struct Vertex *cur = graph->vertices[from];
    if (cur == NULL)
    {
        graph->vertices[from] = malloc(sizeof(struct Vertex));
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
    cur->next->next = NULL;
    cur->next->number = to;
    return;
}

void printGraph(struct Graph graph)
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