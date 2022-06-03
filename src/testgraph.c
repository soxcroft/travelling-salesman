/**
 * @file    testgraph.c
 * @brief   A driver program to test the graph implementation.
 * @author  L. Foxcroft
 * @date    2022-06-03
 */

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/*--- main routine -----------------------------------------------------------*/

int main()
{
	Graph *graph = scan_graph();
	print_graph(graph);
	free_graph(graph);
	return EXIT_SUCCESS;
}

