/**
 * @file    stack.c
 * @brief   A stack of partial tours.
 * @author  L. Foxcroft
 * @date    2022-06-03
 */

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

/** a partial tour container */
struct partial_tour {
	/** array of cities visited in partial tour (in order) */
	int *cities;
	/** number of cities in partial tour */
	int count;
	/** sum of the weights of edges traversed in partial tour */
	int cost;
	/** the maximum number of cities which can be visited */
	int max_count;
	/** array of visited status */
	int *visited;
};

/** a stack of partial tours container */
struct stack {
	/** number of tours on the stack */
	int size;
	/** maximum number of tours which can be stored on stack */
	int max_size;
	/** array of pointers to partial tours */
	Partial_tour **tours;
};

/*--- stack interface --------------------------------------------------------*/

Partial_tour *tour_init(int n)
{
	/* allocate space for tour variable */
	Partial_tour *tour = (Partial_tour *) malloc(sizeof(Partial_tour));

	/* set up initial values for partial tour */
	tour->cities = (int *) malloc(sizeof(int) * n);
	tour->count = 0;
	tour->cost = 0;
	tour->max_count = n;
	tour->visited = (int *) malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		tour->visited[i] = 0;
	}

	return tour;
}

int last_city(Partial_tour *tour)
{
	if (tour->count == 0) {
		return -1;
	} else {
		return tour->cities[tour->count-1];
	}
}

int visited(Partial_tour *tour, int city)
{
	return tour->visited[city];
}

int tour_count(Partial_tour *tour)
{
	return tour->count;
}

int tour_cost(Partial_tour *tour)
{
	return tour->cost;
}

void add_city(Partial_tour *tour, int city, int weight)
{
	/* add city to partial tour and update weight */
	tour->cities[tour->count++] = city;
	tour->cost += weight;
	tour->visited[city] = 1;
}

void remove_city(Partial_tour *tour, int weight)
{
	/* remove last city in partial tour and update cost */
	int city = tour->cities[--tour->count];
	tour->cost -= weight;
	tour->visited[city] = 0;
}

void print_tour(Partial_tour *tour)
{
	for (int i = 0; i < tour->count; i++) {
		printf("%d", tour->cities[i]);
		if (i < tour->count - 1) {
			printf("->");
		}
	}
	printf(" (cost %d)\n", tour->cost);
}

void free_tour(Partial_tour *tour)
{
	free(tour->cities);
	free(tour->visited);
	free(tour);
}

Stack *stack_init(int n)
{
	/* allocate space for the stack variable */
	Stack *stack = (Stack *) malloc(sizeof(Stack));
	
	/* Set up initial values for stack. The number of partial tours on the stack
	 * is capped at n^2/2, and we would rather allocate the memory now than
	 * constantly allocate and free partial tours at the top of the stack. */
	stack->size = 0;
	stack->max_size = n*n/2;
	stack->tours = (Partial_tour **) malloc(sizeof(Partial_tour *) * (n*n/2));
	for (int i = 0; i < stack->max_size; i++) {
		stack->tours[i] = tour_init(n);
	}

	return stack;
}

int stack_size(Stack *stack)
{
	return stack->size;
}

void push_copy(Stack *stack, Partial_tour *tour)
{
	/* get pointer to top of stack and update stack size */
	Partial_tour *copy = stack->tours[stack->size++];

	/* copy specified tour to the new top of the stack */
	copy->cost = tour->cost;
	copy->count = tour->count;
	for (int i = 0; i < tour->count; i++) {
		copy->cities[i] = tour->cities[i];
	}
	for (int i = 0; i < tour->max_count; i++) {
		copy->visited[i] = tour->visited[i];
	}
}

void pop(Stack *stack, Partial_tour *tour)
{
	/* update the top of the stack */
	Partial_tour *copy = stack->tours[--stack->size];

	/* copy the tour which was on top of the stack to 'tour' */
	tour->cost = copy->cost;
	tour->count = copy->count;
	for (int i = 0; i < copy->count; i++) {
		tour->cities[i] = copy->cities[i];
	}
	for (int i = 0; i < copy->max_count; i++) {
		tour->visited[i] = copy->visited[i];
	}
}

void pop_front(Stack *stack, Partial_tour *tour)
{
	/* remove the bottom element from the stack */
	Partial_tour *copy = stack->tours[0];
	for (int i = 0; i < stack->size - 1; i++) {
		stack->tours[i] = stack->tours[i+1];
	}
	stack->tours[--stack->size] = copy;

	/* copy to tour */
	tour->cost = copy->cost;
	tour->count = copy->count;
	for (int i = 0; i < copy->count; i++) {
		tour->cities[i] = copy->cities[i];
	}
	for (int i = 0; i < copy->max_count; i++) {
		tour->visited[i] = copy->visited[i];
	}
}

void split_stack(Stack *old_stack, Stack *new_stack)
{
	Partial_tour *ptr;
	for (int i = 1; i < old_stack->size; i += 2) {
		/* copy tour to new stack */
		push_copy(new_stack, old_stack->tours[i-1]);
		/* swap the next item which will stay in old stack with gap created */
		ptr = old_stack->tours[i/2];
		old_stack->tours[i/2] = old_stack->tours[i];
		old_stack->tours[i] = ptr;
	}
	/* update the original stacks size */
	old_stack->size = old_stack->size - new_stack->size;
}

void print_stack(Stack *stack)
{
	printf("size %d\n", stack->size);
	for (int i = 0; i < stack->size; i++) {
		printf("%d: ", i);
		print_tour(stack->tours[i]);
	}
}

void free_stack(Stack *stack)
{
	for (int i = 0; i < stack->max_size; i++) {
		free_tour(stack->tours[i]);
	}
	free(stack->tours);
	free(stack);
}

