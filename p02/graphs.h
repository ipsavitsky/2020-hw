#ifndef GRAPHS_C_
#define GRAPHS_C_

/**
 * \brief structure of a graph
 * \todo maybe rework? the way to store a graph is too convoluted
 */

struct Graph
{
    struct Vertex **vertices; /**< an array of l1 lists to store relations*/
    int vernum; /**< store the amount of verices in a graph*/
};

/**
 * \brief structure of a vertex l1 list
 */

struct Vertex
{
    int number; /**< */
    struct Vertex *next; /**< */
    //int weight;
};

struct Graph uploadGraph(char *filename);

/**
 * \brief print graph in a readable manner
 * \param graph graph that needs to be printed
 */

void printGraph(struct Graph graph);

/**
 * \brief add a correlation between two vertices
 * \param graph which graph to add an edge
 * \param from where the start of a vertex is
 * \param to where the end of a vertex is
 */

void addEdge(struct Graph *graph, int from, int to);

void delete_graph(struct Graph *graph);
#endif