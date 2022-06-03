/**
 * @file    graph.h
 * @brief   An adjacency list implementation for an undirected graph.
 * @author  L. Foxcroft
 * @date    2022-06-03
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "boolean.h"

/** the container structure for a node */
typedef struct node Node;

/** the container structure for a graph */
typedef struct graph Graph;

/*--- function prototypes ----------------------------------------------------*/

/**
 * Scan a weighted undirected graph from standard in and return its adjacency
 * list representation.
 *
 * @return      a pointer to the graph allocated
 */
Graph *scan_graph();

/**
 * Prints a graph to standard out.
 *
 * @param[in]   graph
 *      a pointer to the graph to display
 */
void print_graph(Graph *graph);

/**
 * Frees the space associated with the specified graph structure
 *
 * @param[in]   graph
 *      a pointer to the graph to free
 */
void free_graph(Graph *graph);

#endif /* GRAPH_H */
