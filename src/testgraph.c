/**
 * @file    testgraph.c
 * @brief   A driver program to test the graph implementation.
 * @author  L. Foxcroft
 * @date    2022-06-03
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

#define BUFFERSIZE 100

/*--- main routine -----------------------------------------------------------*/

int main()
{
	char buffer[BUFFERSIZE];
	int city, neighbour, cost, visited;
	Graph *graph = NULL;

	printf("Type \"quit <Enter>\" to exit\n");
	printf("Type \"scan <Enter>\" to read in a new graph\n");
	printf("Type \"print <Enter>\" to print the current graph\n");
	printf("Type \"adj <city> <Enter>\" to see which nodes are adjacent to a city\n");

	printf(">> ");
	scanf("%s", buffer);
	while (strcmp(buffer, "quit") != 0) {
		if (strcmp(buffer, "scan") == 0) {
			if (graph != NULL) {
				free_graph(graph);
			}
			graph = scan_graph();
		} else if (strcmp(buffer, "print") == 0 && graph != NULL) {
			printf("Graph:\n");
			print_graph(graph);
		} else if (strcmp(buffer, "adj") == 0 && graph != NULL) {
			scanf("%d", &city);
			printf("%d: ", city);
			visited = adj(graph, &city, &neighbour, &cost);
			while (visited) {
				printf("%d (%d), ", neighbour, cost);
				visited = adj(graph, NULL, &neighbour, &cost);
			}
			printf("\n");
		}
		printf(">> ");
		scanf("%s", buffer);
	}

	if (graph != NULL) {
		free_graph(graph);
	}

	return EXIT_SUCCESS;
}

