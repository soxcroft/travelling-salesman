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
 * Allocates and returns a graph with v vertices, e edges, which are specified
 * by the edges array (edges[i][0] is connected to edges[i][1] with weight
 * edges[i][2]). Added because it will be much easier to send an array to a
 * process that they can build the graph with than the graph structure itself.
 *
 * @param[in]   v
 *     the number of vertices in the graph
 * @param[in]   e
 *     the number of edges in the graph
 * @param[in]   edges
 *     an array of edges and edge weights
 * @return      an adjacency list representation of the specified graph
 */
Graph *build_graph(int v, int e, int **edges);

/**
 * If there is a another node adjacent to city, read its destination into
 * neighbour and distance to it into cost.
 *
 * @param[in]   graph
 *     a pointer to the underlying graph
 * @param[in]   city
 *     the city whose neighbours we would like to visit
 * @param[out]  neighbour
 *     the next neighbour of specified city
 * @param[out]  cost
 *     the weight of the edge between the city and its next neighbour
 * @return      true if a neighbour was visited, else false
 */
Boolean adj(Graph *graph, int *city, int *neighbour, int *cost);

/**
 * Prints a graph to standard out.
 *
 * @param[in]   graph
 *     a pointer to the graph to display
 */
void print_graph(Graph *graph);

/**
 * Frees the space associated with the specified graph structure.
 *
 * @param[in]   graph
 *     a pointer to the graph to free
 */
void free_graph(Graph *graph);

#endif /* GRAPH_H */
