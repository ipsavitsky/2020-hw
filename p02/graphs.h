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
 * \warning this function is cringe
 * \todo document the function better to make it less cringe
 */
void remove_vertex(struct Graph *graph, int vertex);

/**
 * \brief remove an edge from the graph
 * \param graph graph from which the edge should be removed
 * \param from 
 * \param to
 */
void remove_edge(struct Graph *graph, int from, int to);

/**
 * \brief counts the amount of paths between two vertices
 * \param graph the graph in which the ways are searched
 * \param from the start of the path
 * \param to the end of path
 * \param control USED INTERNALLY IN RECURSION
 * \warning do not call this function with something other then a blanc array of int of size graph.vernum
 */
int way_count(struct Graph graph, int from, int to, int *control);

/**
 * \brief initialize a blanc path structure needed for correct recursion in print_all_paths()
 * \param len length parameter USED INTERNALLY
 * \warning DO NOT LAUNCH WITH ANYTHING OTHER THAN 1 IN len
 */
struct Path *init_path(int len);

/**
 * \brief print all paths from from vertex to to vertex
 * \param graph the graph in which to search paths
 * \param from the start of path
 * \param to the end of path
 * \param curpath USED INTERNALLY
 * \warning DO NOT LAUNCH WITH ANYTHIN OTHER THAN A PATH CREATED BY init_path(1)
 */
void print_all_paths(struct Graph graph, int from, int to, struct Path* curpath);
#endif