#ifndef _UTILS_H
#define _UTILS_H

#include <signal.h>
#include <stdlib.h>

/* Custom signals */
#define SIGDAY SIGUSR1
#define SIGSWELL SIGUSR2
#define SIGSTORM SIGUSR2
#define SIGMAELSTROM SIGINT


typedef int bool_t;
#define TRUE 1
#define FALSE 0

typedef struct{
	struct cargo_node *head;
} cargo_hold;

void insert_cargo(cargo_hold *cargo);

/**
 * @return a random double between min and max.
 */
#define RANDOM_DOUBLE(min, max)\
	(rand() / (double)RAND_MAX * (max - min) + (min))

#endif
