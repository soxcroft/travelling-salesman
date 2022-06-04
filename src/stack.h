/**
 * @file    stack.h
 * @brief   A stack which can store partial tours of the travelling salesman.
 * @author  L. Foxcroft
 * @date    2022-06-03
 */

#ifndef TOUR_H
#define TOUR_H

/** the structure for a partial tour */
typedef struct partial_tour Partial_tour;

/** the container structure for a stack of partial tours */
typedef struct stack Stack;

/*--- function prototypes ----------------------------------------------------*/

/**
 * Initialises a partial tour.
 *
 * @param[in]   n
 *     the number of cities in the graph associated with this tour
 */
Partial_tour *tour_init(int n);

/**
 * Adds a city to the specified partial tour.
 *
 * @param[in]   tour
 *     a pointer to the tour whose route we should append to
 * @param[in]   city
 *     the city which should be added to the partial tour
 * @param[in]   weight
 *     the weight of the edge which leads to the city
 */
void add_city(Partial_tour *tour, int city, int weight);

/**
 * Removes the last city visited in the specified partial tour.
 *
 * @param[in]   tour
 *     a pointer to the tour whose last city should be removed
 * @param[in]   weight
 *     the weight of the edge which was traversed to get to the last city
 */
void remove_city(Partial_tour *tour, int weight);

/**
 * Displays the specified partial tour on standard output.
 *
 * @param[in]   tour
 *     the partial tour which should be printed.
 */
void print_tour(Partial_tour *tour);

/**
 * Frees the space associated with the specified partial tour.
 *
 * @param[in]   tour
 *     the partial tour to free
 */
void free_tour(Partial_tour *tour);

/**
 * Allocates memory for and returns a stack which should be able to store all
 * the partial tours stored during a depth first search of a graph with n
 * cities. There shouldn't ever be more than n^2/2 tours on the stack.
 *
 * @param[in]   n
 *     the number of cities in the problem
 * @return      a pointer to a stack structure
 */
Stack *stack_init(int n);

/**
 * Copies the data associated with the partial tour and adds it to the top of
 * the stack.
 *
 * @param[in]   stack
 *     a pointer to the stack
 * @param[in]  tour
 *     the partial tour which should be added to the stack
 */
void push_copy(Stack *stack, Partial_tour *tour);

/**
 * Pops the top element from the stack and copies the data to the partial tour
 * structure pointed to by tour.
 *
 * @param[in]   stack
 *     the stack whose top element should be removed
 * @param[out]  tour
 *     a pointer to the partial tour where the top element can be copied to
 */
void pop(Stack *stack, Partial_tour *tour);

/**
 * Pops the bottom element from the stack and copies the data to the partial tour
 * structure pointed to by tour. A bit ad hoc and inefficient but it will only
 * get used when running a BFS from the inital position to generate enough
 * partial tours to distribute among all the threads.
 *
 * @param[in]   stack
 *     the stack whose bottom element should be removed
 * @param[out]  tour
 *     a pointer to the partial tour where the bottom element can be copied to
 */
void pop_front(Stack *stack, Partial_tour *tour);

/**
 * Splits the partial tours in old_stack between old_stack and new_stack in a
 * cyclic fashion. This is so that the work is distributed somewhat evenly.
 *
 * @param[in,out] old_stack
 *     the stack which should be split
 * @param[out]    new_stack
 *     an empty stack which we should add elements to
 */
void split_stack(Stack *old_stack, Stack *new_stack);

/**
 * Displays the specified stack on standard output.
 *
 * @param[in]   stack
 *     the stack which should be printed.
 */
void print_stack(Stack *stack);

/**
 * Frees the space associated with the specified stack.
 *
 * @param[in]   stack
 *     the stack to free
 */
void free_stack(Stack *stack);

#endif /* TOUR_H */
