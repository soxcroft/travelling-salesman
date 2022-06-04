/**
 * @file    tsp.c
 * @brief   Computes a solution for a travelling salesman problem with MPI.
 * @author  L. Foxcroft
 * @date    2022-06-04
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
//#include <mpich/mpi.h>
#include <mpi.h>
#include "graph.h"
#include "stack.h"

#define BUFFER_SIZE 1000

/*--- debugging --------------------------------------------------------------*/

#ifdef DEBUG
	void debug_edge_list(int v, int e, int **edges, int rank);
	void debug_graph(Graph *graph, int rank);
	void debug_stack(Stack *stack, int rank);
	#define DBG_edge_list(...) debug_edge_list(__VA_ARGS__)
	#define DBG_graph(...) debug_graph(__VA_ARGS__)
	#define DBG_stack(...) debug_stack(__VA_ARGS__)
#else
	#define DBG_edge_list(...)
	#define DBG_graph(...)
	#define DBG_stack(...)
#endif /* DEBUG */

/*--- function prototypes ----------------------------------------------------*/

int **init_edge_list(int e);
void scan_edge_list(int *v, int *e, int ***edges);
void free_edge_list(int e, int **edges);
Stack *generate_subproblems(Graph *graph, int comm_sz, int num_cities);
Stack *select_subproblems(Stack *stack, int comm_sz, int my_rank,
		int num_cities);
Partial_tour *find_best_tour(Graph *graph, Stack *subproblems, int num_cities);
void send_edge_list(int v, int e, int **edges);
void recv_edge_list(int *v, int *e, int ***edges);

/*--- main routine -----------------------------------------------------------*/

int main()
{
	int my_rank = 0, comm_sz = 0;
	int v, e, **edges, cur_tour, min_tour;
	Graph *graph;
	Partial_tour *tour;
	Stack *stack;

	/* Start up MPI */
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank == 0) {
		/* scan and share edge list */
		scan_edge_list(&v, &e, &edges);
		DBG_edge_list(v, e, edges, my_rank);
		send_edge_list(v, e, edges);
	} else {
		/* receive edge list from process 0 */
		recv_edge_list(&v, &e, &edges);
	}

	/* every process should build graph */
	graph = build_graph(v, e, edges);
	DBG_graph(graph, my_rank);

	/* bfs to find enough subproblems for each process, and pick subproblems
	 * based on rank */
	stack = generate_subproblems(graph, comm_sz, v);
	DBG_stack(stack, my_rank);
	stack = select_subproblems(stack, comm_sz, my_rank, v);
	DBG_stack(stack, my_rank);

	/* find the best tour from process's subproblems */
	tour = find_best_tour(graph, stack, v);
	if (tour == NULL) {
		cur_tour = INT_MAX;
	} else {
		cur_tour = tour_cost(tour);
	}

	/* send length of best tour to process 0 TODO send tour too */
	MPI_Reduce(&cur_tour, &min_tour, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

	if (my_rank == 0) {
		printf("%d\n", min_tour);
	}

	/* release allocated resources */
	free_edge_list(e, edges);
	free_graph(graph);
	free_stack(stack);
	free_tour(tour);

	/* Shut down MPI */
	MPI_Finalize();

	return EXIT_SUCCESS;
}

/*--- utility functions ------------------------------------------------------*/

/** Allocates memory for and returns a 2D integer array which can represent an
 * edge list containing e edges. */
int **init_edge_list(int e)
{
	int **edges = (int **) malloc(sizeof(int *) * e);
	for (int i = 0; i < e; i++) {
		edges[i] = (int *) malloc(sizeof(int) * 3);
	}
	return edges;
}

/** Read graph from standard in into an edge list. */
void scan_edge_list(int *v, int *e, int ***edges)
{
	scanf("%d %d", v, e);
	*edges = init_edge_list(*e);
	for (int i = 0; i < *e; i++) {
		scanf("%d %d %d", &(*edges)[i][0], &(*edges)[i][1], &(*edges)[i][2]);
	}
}

/** Free the memory associated with specified edge list. */
void free_edge_list(int e, int **edges)
{
	for (int i = 0; i < e; i++) {
		free(edges[i]);
	}
	free(edges);
}

/** Add initial subproblem to the specified stack and run a breadth first search
 * until there are at least comm_sz (communication size) subproblems on the
 * stack. Otherwise we can't give every thread work. */
Stack *generate_subproblems(Graph *graph, int comm_sz, int num_cities)
{
	int city, neighbour, cost, search;
	Partial_tour *tour;
	Stack *stack;

	/* create stack and add initial subproblem (salesman at city 0) */
	stack = stack_init(num_cities);
	tour = tour_init(num_cities);
	add_city(tour, 0, 0);
	push_copy(stack, tour);

	/* bfs */
	while (stack_size(stack) > 0 && stack_size(stack) < comm_sz) {
		pop_front(stack, tour);
		city = last_city(tour);
		if (city != -1) {
			search = adj(graph, &city, &neighbour, &cost);
			while (search) {
				if (!visited(tour, neighbour)) {
					add_city(tour, neighbour, cost);
					push_copy(stack, tour);
					remove_city(tour, cost);
				}
				search = adj(graph, NULL, &neighbour, &cost);
			}
		}
	}

	free_tour(tour);

	return stack;
}

/** Select the problems on the stack in a cyclic fashion based on process rank.
 * All threads should thus generate the same set of subproblems and pick their
 * ones, rather than the master thread distributing them. */
Stack *select_subproblems(Stack *stack, int comm_sz, int my_rank,
		int num_cities)
{
	int cnt;
	Partial_tour *tour;
	Stack *my_problems; 

	/* initialize new stack, a tour to help copy this threads subproblems to new
	 * stack, and a counter to keep track of iterations */
	my_problems = stack_init(num_cities);
	tour = tour_init(num_cities);
	cnt = 0;

	/* remove problems from old stack, adding them to my_problems every comm_sz
	 * iterations, with my_rank offset so that problems are unique to process
	 * with unique rank */
	while (stack_size(stack) > 0) {
		pop(stack, tour);
		if (cnt % comm_sz == my_rank) {
			push_copy(my_problems, tour);
		}
		cnt++;
	}

	free_tour(tour);
	free_stack(stack);

	return my_problems;
}

/** Search for the best tour from the initial subproblems that the process
 * should have computed. */
Partial_tour *find_best_tour(Graph *graph, Stack *subproblems, int num_cities)
{
	int city, neighbour, cost, search;
	Partial_tour *best_tour, *helper_tour, *tour_ptr;

	/* initialize tours, helper gets written to during search */
	best_tour = tour_init(num_cities);
	add_city(best_tour, 0, INT_MAX); /* indicates that a tour is not possible */
	helper_tour = tour_init(num_cities);
	
	/* iterative dfs */
	while (stack_size(subproblems) > 0) {
		pop(subproblems, helper_tour);
		city = last_city(helper_tour);
		if (city != -1) { /* ie partial tour is not empty */
			search = adj(graph, &city, &neighbour, &cost);
			while (search) {
				/* add 0 to finish tour if we have visited every city */
				if (tour_count(helper_tour) == num_cities && neighbour == 0) {
					add_city(helper_tour, neighbour, cost);
					if (tour_cost(helper_tour) < tour_cost(best_tour)) {
						/* swap pointers */
						tour_ptr = best_tour;
						best_tour = helper_tour;
						helper_tour = tour_ptr;
					}
				}
				/* else continue search by visiting neighbouring cities */
				else if (!visited(helper_tour, neighbour)) {
					add_city(helper_tour, neighbour, cost);
					push_copy(subproblems, helper_tour);
					remove_city(helper_tour, cost);
				}
				/* get next neighbour in linked list */
				search = adj(graph, NULL, &neighbour, &cost);
			}
		}
	}

	free_tour(helper_tour);

	return best_tour;
}


/*--- messaging functions ----------------------------------------------------*/

// TODO do some checks so that buffers don't overflow

/** Broadcast the number of vertices, edges and edge list to the other processes
 * so that they can reconstruct the graph scanned from standard in */
void send_edge_list(int v, int e, int **edges)
{
	int position = 0;
	int send_buffer[BUFFER_SIZE];

	/* pack vertices and edges into buffer */
	MPI_Pack(&v, 1, MPI_INT, send_buffer, BUFFER_SIZE, &position,
			MPI_COMM_WORLD);
	MPI_Pack(&e, 1, MPI_INT, send_buffer, BUFFER_SIZE, &position,
			MPI_COMM_WORLD);

	/* pack the edge list into a contiguous array (because it's easier to
	 * send than a 2d array) and broadcast to worker threads */
	for (int i = 0; i < e; i++) {
		MPI_Pack(edges[i], 3, MPI_INT, send_buffer, BUFFER_SIZE, &position,
				MPI_COMM_WORLD);
	}

	/* broadcast to worker threads */
	MPI_Bcast(send_buffer, position, MPI_PACKED, 0, MPI_COMM_WORLD);
}

/** Receive the number of vertices, edges and edge list that the master thread
 * scanned. Then update the worker's values. */
void recv_edge_list(int *v, int *e, int ***edges)
{
	int position = 0;
	int recv_buffer[BUFFER_SIZE];

	/* receive vertices, edges and edge list */
	MPI_Bcast(recv_buffer, BUFFER_SIZE, MPI_PACKED, 0, MPI_COMM_WORLD);

	/* unpack number of vertices and edges */
	MPI_Unpack(recv_buffer, BUFFER_SIZE, &position, v, 1, 
			MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(recv_buffer, BUFFER_SIZE, &position, e, 1, 
			MPI_INT, MPI_COMM_WORLD);

	/* allocate memory for edge list and unpack into it */
	*edges = init_edge_list(*e);
	for (int i = 0; i < *e; i++) {
		MPI_Unpack(recv_buffer, BUFFER_SIZE, &position, (*edges)[i], 3, 
				MPI_INT, MPI_COMM_WORLD);
	}
}

/*--- debugging output -------------------------------------------------------*/

#ifdef DEBUG

void debug_edge_list(int v, int e, int **edges, int rank)
{
	if (rank == 0) {
		printf("EDGE LIST\n");
		printf("%d %d\n", v, e);
		for (int i = 0; i < e; i++) {
			printf("%d %d %d\n", edges[i][0], edges[i][1], edges[i][2]);
		}
	}
}

void debug_graph(Graph *graph, int rank)
{
	if (rank == 0) {
		printf("GRAPH\n");
		print_graph(graph);
	}
}

void debug_stack(Stack *stack, int rank)
{
	if (rank == 0) {
		printf("STACK\n");
		print_stack(stack);
	}
}

#endif

