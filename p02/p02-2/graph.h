#include <stddef.h>

/**
 * enum типов которые поддерживает граф. Типы сюда можно добавлять(если при этом
 * добавить соответствующую обработку в switch во всей программе)
 */
enum Types { INTEGER, DOUBLE, CHAR };

/**
 * \brief структура ноды графа
 */
typedef struct Graph_node {
    void *data;                /**< данные, хранящиеся в ноде */
    size_t data_size;          /**< КОЛИЧЕСТВО элементов в data */
    enum Types data_type;      /**< тип данных в data */
    struct Graph_node **edges; /**< указатели на элементы с которыми есть ребра у данного элемента*/
    size_t num_edges;          /**< количество ребер, исходящих ИЗ вершины*/
    unsigned short existent;   /**< флаг существования вершины, 0 - не существует, все остальное - существует*/
} graph_node;

/**
 * структура графа
 */
typedef struct {
    size_t num_vertices;  /**< количество нод графа*/
    graph_node *vertices; /**< массив нод графа*/
} graph;

/**
 * тип функций, применяемых на данные конкретной ноды
 */
typedef void (*data_func)(void *, size_t, enum Types);

/**
 * \brief загрузка графа из бинарного файла
 * \param filename имя файла из которого загружать граф
 * \return граф, загруженный из файла
 * \warning после этой функции требуется перемэтчить вершины(см. match_nodes)
 */
graph upload_graph(char *filename);

/**
 * \brief сохранение графа в файл
 * \param filename название файла в который сохраняется файл
 * \param sv_graph граф который надо сохранить
 */
void save_graph(char *filename, graph sv_graph);

/**
 * \brief печать графа в stdout
 * \param in_graph граф который надо распечатать
 */
void print_graph(graph in_graph);

/**
 * \brief добавить вершину в граф
 * \param in_graph граф в который надо добавить вершину
 * \param data_type тип данных в вершине
 * \param data указатель на данные которые надо сохранить
 * \param data_size количество элементов в data
 * \warning после этой функции требуется перемэтчить вершины(см. match_nodes)
 */
void add_vertex(graph *in_graph, enum Types data_type, void *data,
                       size_t data_size);

/**
 * \brief создать ребро между двумя вершинами
 * \param node1 откуда выходит реброкоторое надо удалить
 * \param node2 куда идет это ребро
 */
void add_edge(graph_node *node1, graph_node *node2);

/**
 * \brief вывести граф в файл в формате, который понимает graphviz
 * \param pr_graph граф на вывод
 * \param filename имя файла для вывода
 */
void graphviz_output(graph pr_graph, char *filename);

/**
 * \brief удаление графа из памяти
 * \param del_graph граф который надо удалить
 */
void delete_graph(graph *del_graph);

/**
 * \brief удаление ребра из графа
 * \param node1 вершина из которой надо удалить ребро
 * \param node2 вершина в которую идет удаляемое ребро
 */
void delete_edge(graph_node *node1, graph_node *node2);

/**
 * \brief удаление вершины из графа
 * \param gr граф из которого надо удалить вершину
 * \param node вершина которую надо удалить
 * \warning после этой функции требуется перемэтчить вершины(см. match_nodes)
 */
void delete_vertex(graph *gr, graph_node *node);

/**
 * \brief удаление кратных ребер в графе
 * \param gr граф в котором удаляются кратные ребра
 */
void reduce_connections(graph *gr);

/**
 * \brief обоход графа "по массиву" и применение функции к данным каждой вершины
 * \param gr граф который надо обойти
 * \param function функция которую надо применить к каждой вершине
 */
void traverse(graph *gr, data_func function);


/**
 * \brief функция мэтчинга вершин графа. Она автоматически выделяет место под все существующие вершины и настраивает 
 * ссылки
 * \param res массив в который надо записать замэтченные вершины
 * \param gr граф вершины которого мэтчим
 * \return массив в который надо записать замэтченные вершины
 */
graph_node **match_vertices(graph_node **res, graph gr);