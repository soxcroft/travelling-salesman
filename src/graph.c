/**
 * @file    graph.c
 * @brief   Adjacency list representation of an undirected graph.
 * @author  L. Foxcroft
 * @date    2022-06-03
 */

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/** a node container */
struct node {
	/** represents the city in this problem */
	int dest;
	/** weight of the edge from src to dest if node is in list indexed by src */
	int dist;
	/** next node adjacent to src in linked list */
	struct node *next;
};

/** a graph container */
struct graph {
	/** the number of vertices in the graph */
	int vertices;
	/** a list of pointers to the adjacency lists */
	struct node **nodes;
};

/*--- function prototypes ----------------------------------------------------*/

static Graph *graph_init(int vertices);
static Boolean graph_add_edge(Graph *graph, int from, int to, int weight);

/*--- graph interface --------------------------------------------------------*/

Graph *scan_graph()
{
	int v, e, v1, v2, w;
	scanf("%d %d", &v, &e);
	Graph *graph = graph_init(v);
	for (int i = 0; i < e; i++) {
		scanf("%d %d %d", &v1, &v2, &w);
		if (!graph_add_edge(graph, v1, v2, w)) {
			printf("Could not add edge from %d to %d with weight %d\n",
					v1, v2, w);
		}
		if (!graph_add_edge(graph, v2, v1, w)) {
			printf("Could not add edge from %d to %d with weight %d\n",
					v2, v1, w);
		}
	}
	return graph;
}

Graph *build_graph(int v, int e, int **edges)
{
	Graph *graph = graph_init(v);
	for (int i = 0; i < e; i++) {
		if (!graph_add_edge(graph, edges[i][0], edges[i][1], edges[i][2])) {
			printf("Could not add edge from %d to %d with weight %d\n",
					edges[i][0], edges[i][1], edges[i][2]);
		}
		if (!graph_add_edge(graph, edges[i][1], edges[i][0], edges[i][2])) {
			printf("Could not add edge from %d to %d with weight %d\n",
					edges[i][1], edges[i][0], edges[i][2]);
		}
	}
	return graph;
}

Boolean adj(Graph *graph, int *city, int *neighbour, int *cost)
{
	static Node *n;

	if (city != NULL) {
		if (*city < 0 || *city >= graph->vertices) {
			return FALSE;
		}
		n = graph->nodes[*city];
	} /* else it has already been cached */

	/* return if next adjacent node does not exist */
	if (n == NULL) {
		return FALSE;
	}

	/* read values of adjacent node into pointers */
	*neighbour = n->dest;
	*cost = n->dist;
	n = n->next;

	return TRUE;
}

void print_graph(Graph *graph)
{
	for (int i = 0; i < graph->vertices; i++) {
		printf("%d: ", i);
		Node *n = graph->nodes[i];
		while (n != NULL) {
			printf("%d (%d), ", n->dest, n->dist);
			n = n->next;
		}
		printf("\n");
	}
}

void free_graph(Graph *graph)
{
	int i;
	Node *p, *q;

	/* Free linked lists pointed to by graph */
	for (i = 0; i < graph->vertices; i++) {
		p = graph->nodes[i];
		while (p != NULL) {
			q = p->next;
			free(p);
			p = q;
		}
	}

	/* Free graph */
	free(graph->nodes);
	free(graph);
}

/*--- utility functions ------------------------------------------------------*/

/** Initialize an adjacency list with 'vertices' nodes */
static Graph *graph_init(int vertices)
{
	Graph *graph = (Graph *) malloc(sizeof(Graph));
	graph->vertices = vertices;
	graph->nodes = (Node **) malloc(sizeof(Node **) * vertices);
	for (int i = 0; i < vertices; i++) {
		graph->nodes[i] = NULL;
	}
	return graph;
}

/** Add an edge from 'from' to 'to' with specified weight */
static Boolean graph_add_edge(Graph *graph, int from, int to, int weight)
{
	if (from >= graph->vertices || to >= graph->vertices) {
		return FALSE;
	}

	/* Add node to front of linked list indexed by from */
	Node *n1 = (Node *) malloc(sizeof(Node));

	if (n1 == NULL) {
		return FALSE;
	}

	n1->dest = to;
	n1->dist = weight;
	n1->next = graph->nodes[from];
	graph->nodes[from] = n1;

	return TRUE;
}

