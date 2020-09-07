#ifndef GRAPHS_C_
#define GRAPHS_C_

/**
 * \file
 * \brief graph header
 */

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
    int number; /**< storing an end to an edge*/
    struct Vertex *next; /**< pointer to next in list structure*/
    //int weight;
};

/**
 * \brief initialize graph from file
 * \param filename name of the file from which to upload
 * \return graph which the matrix determines
 */

struct Graph upload_graph(char *filename);

/**
 * \brief print graph in a readable manner
 * \param graph graph that needs to be printed
 */

void print_graph(struct Graph graph);

/**
 * \brief add a correlation between two vertices
 * \param graph which graph to add an edge
 * \param from where the start of a vertex is
 * \param to where the end of a vertex is
 */

void add_edge(struct Graph *graph, int from, int to);

/**
 * \brief delete graph and all of its members
 * \param graph graph to delete
 */
void delete_graph(struct Graph *graph);

/**
 * \brief add a vertex to a graph
 * \param graph graph to which the vertex should be added
 */
void add_vertex(struct Graph *graph);

/**
 * \brief remove a vertex from a graph
 * \param graph graph from which the vertex should be removed
 * \param vertex number of vertex that should be removed
 * \todo this function is cringe
 */
void remove_vertex(struct Graph *graph, int vertex);
#endif