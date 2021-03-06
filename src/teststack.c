/**
 * @file    teststack.c
 * @brief   A driver program to test the stack implementation.
 * @author  L. Foxcroft
 * @date    2022-06-03
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

#define BUFFERSIZE 100
#define N 10

/*--- main routine -----------------------------------------------------------*/

int main() {
	char buffer[BUFFERSIZE];
	int cities, city, weight;
	Partial_tour *tour = tour_init(N);
	Stack *stack = stack_init(N);
	Stack *helper_stack = stack_init(N);

	add_city(tour, 0, 0);
	push_copy(stack, tour);

	printf("Actions\n=======\n");
	printf("print                            -- display the partial tours on the stack\n");
	printf("push <cities> <city> <weight>... -- add <cities> copies of top of stack with <city> and <weight> appended to it\n");
	printf("pop                              -- remove the top tour from the stack \n");
	printf("pop_front                        -- remove the bottom tour from the stack\n");
	printf("split                            -- split the stack in half\n");
	printf("quit                             -- quit program\n");

	printf(">> ");
	scanf("%s", buffer);
	while (strcmp(buffer, "quit") != 0) {
		if (strcmp(buffer, "push") == 0) {
			printf(">> cities: ");
			scanf("%d", &cities);
			pop(stack, tour);
			push_copy(stack, tour);
			for (int i = 0; i < cities; i++) {
				printf(">> city: ");
				scanf("%d", &city);
				printf(">> weight: ");
				scanf("%d", &weight);
				add_city(tour, city, weight);
				push_copy(stack, tour);
				printf("Pushed: ");
				print_tour(tour);
				remove_city(tour, weight);
			}
		} else if (strcmp(buffer, "pop") == 0) {
			pop(stack, tour);
			printf("Popped: ");
			print_tour(tour);
		} else if (strcmp(buffer, "pop_front") == 0) {
			pop_front(stack, tour);
			printf("Popped: ");
			print_tour(tour);
		} else if (strcmp(buffer, "split") == 0) {
			split_stack(stack, helper_stack);
			printf("Old stack:\n");
			print_stack(stack);
			printf("New stack:\n");
			print_stack(helper_stack);
		} else if (strcmp(buffer, "print") == 0) {
			print_stack(stack);
		}
		printf(">> ");
		scanf("%s", buffer);
	}

	free_tour(tour);
	free_stack(stack);
	free_stack(helper_stack);

	return EXIT_SUCCESS;
}

