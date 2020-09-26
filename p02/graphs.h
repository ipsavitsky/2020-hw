#ifndef GRAPHS_C_
#define GRAPHS_C_

/**
 * \file
 * \brief graph header
 */

/**
 * Each vertex in a graph is an element in vertices array, but the
 * each element of this array is practically an l1 list of relations
 * So this graph:
 * ```
 * 0+--2            
 * +  /+            
 * | / |            
 * |+  |            
 * 1+--3
 * ```
 * (where '+' is a representation of an arrow) \n
 * Will translate to this: \n
 * ```
 * [0]: NULL
 * [1]: 0 -> NULL
 * [2]: 0 -> 1 -> NULL
 * [3]: 1 -> 2 -> NULL
 * ```
 * \brief structure of a graph
 * \todo maybe rework? the way to store a graph is too convoluted
 */
struct Graph
{
    struct Vertex **vertices; /**< an array of l1 lists to store edges*/
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
 * Initializes a graph from a file of a following structure: \n
 * First number is the amount of vertices \n
 * Then follows n x n matrix of 0 and 1, where 1 means a prescense of an edge and 0 is its absence
 * This way 
 * ```
 * 4
 * 0 0 0 0
 * 1 0 0 0
 * 1 1 0 0
 * 0 1 1 0
 * ``` 
 * will be translated to:
 * ```
 * [0]: NULL
 * [1]: 0 -> NULL
 * [2]: 0 -> 1 -> NULL
 * [3]: 1 -> 2 -> NULL
 * ```
 * 
 * \brief initialize graph from file
 * \param filename name of the file from which to upload
 * \return graph which the matrix determines
 * \warning THE FUNCTION TAKES FILE STRUCTURE FOR GRANTED, IT DOES NOT CHECK WHAT IS FED TO IT
 */
struct Graph upload_graph(char *filename);

/**
 * \brief print graph in a readable manner
 * \param graph graph that needs to be printed
 */
void print_graph(struct Graph graph);

/**
 * better way to output graphs, `make out.pdf` takes care of building a pdf file
 * \brief better way to output graphs
 * \param graph graph that needs to be printed
 * \todo make custom filenames
 */
void print_graphviz(struct Graph graph);

/**
 * \brief add a correlation between two vertices
 * \param graph which graph to add an edge
 * \param from where the start of a vertex is
 * \param to where the end of a vertex is
 */
    void add_edge(struct Graph * graph, int from, int to);

/**
 * \brief delete graph and all of its members
 * \param graph graph to delete
 */
    void delete_graph(struct Graph * graph);

/**
 * \brief add a vertex to a graph
 * \param graph graph to which the vertex should be added
 */
    void add_vertex(struct Graph * graph);

/**
 * \brief remove a vertex from a graph
 * \param graph graph from which the vertex should be removed
 * \param vertex number of vertex that should be removed
 * \warning this function is cringe
 * \todo document the function better to make it less cringe
 */
    void remove_vertex(struct Graph * graph, int vertex);

/**
 * \brief remove an edge from the graph
 * \param graph graph from which the edge should be removed
 * \param from - starting point of an edge
 * \param to - ending point of an edge
 */
    void remove_edge(struct Graph * graph, int from, int to);

/**
 * \brief counts the amount of paths between two vertices
 * \param graph the graph in which the ways are searched
 * \param from the start of the path
 * \param to the end of path
 * \param control USED INTERNALLY IN RECURSION
 * \return amount of ways between two vertices
 * \warning do not call this function with something other then a blanc array of int of size graph.vernum
 */
    int way_count(struct Graph graph, int from, int to, int *control);

/**
 * \brief revert all paths in graph
 * \param graph input graph that needs to be reverted
 * \return returns a new, reversrd graph
 */
    struct Graph revert_graph(struct Graph graph);
#endif