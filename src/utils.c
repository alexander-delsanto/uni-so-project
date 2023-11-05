#define _GNU_SOURCE

#include <stdio.h>
#include "header/utils.h"

struct cargo_node {
	int quantity;
	int expiration_date;
	struct cargo_node *next;
};


/*
 * Ideally should insert cargo in the list ordered by expiration date and id.
 */
void insert_cargo(cargo_hold *cargo)
{
	/* TODO */
}
