#ifndef _UTILS_H
#define _UTILS_H

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Custom signals */
#define SIGDAY SIGUSR1
#define SIGSWELL SIGUSR2
#define SIGSTORM SIGUSR2
#define SIGMAELSTROM SIGTERM

typedef int bool_t;
#define TRUE 1
#define FALSE 0

typedef struct {
	int available_quantity;
	struct cargo_node *head;
} cargo_hold;

/**
 * @brief converts time data to timespec and calls nanosleep().
 *
 * @param time_required time required to terminate.
 */
void convert_and_sleep(double time_required);

/**
 * Inserts cargo in the list ordered by expiration date.
 * If expiration date does not exist, a new node is created.
 * @param cargo list of cargo with same id
 * @param quantity quantity to add
 * @param expiration_date
 */
void insert_cargo(cargo_hold *cargo, int quantity, int expiration_date);

/**
 * Extracts cargo from the list ordered by expiration date.
 * If quantity is 0, the node is deleted.
 * @param cargo list of cargo with same id
 * @param quantity quantity to add
 * @param expiration_date
 */
void remove_cargo(cargo_hold *cargo, int quantity, int expiration_date);

/**
 *
 * @param cargo the cargo hold containing the list of cargo to be counted
 * @return the cargo quantity of the type of cargo_hold
 */
int get_cargo_quantity(cargo_hold *cargo);

/**
 * @return a random integer between min and max (included).
 */
#define RANDOM_INTEGER(min, max) ((rand() % ((max) - (min) + 1)) + (min))

/**
 * @return a random double between min and max (included).
 */
#define RANDOM_DOUBLE(min, max) \
	(rand() / (double)RAND_MAX * (max - min) + (min))

#define HOURS_IN_DAY 24.0
#define NANOSECONDS_IN_SECOND \
	1000000000 /* constant for conversion to nanoseconds */

#endif
