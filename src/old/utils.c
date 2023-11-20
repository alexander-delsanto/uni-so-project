#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include "header/utils.h"
#include "header/ipc_utils.h"

struct cargo_node {
	int quantity;
	int expiration_date;
	struct cargo_node *next;
};

void delete_node(struct cargo_node *prev, struct cargo_node *curr);

void insert_cargo(cargo_hold *cargo, int quantity, int expiration_date)
{
	struct cargo_node *prev, *tmp, *curr;

	if (cargo == NULL) {
		dprintf(2, "utils.c: insert_cargo: cargo list is NULL\n");
		return;
	}

	prev = NULL;
	curr = cargo->head;
	while (1) {
		/* If list is empty or expiration date is lower than current element */
		if (curr == NULL || curr->expiration_date > expiration_date) {
			tmp = malloc(sizeof(*curr));
			tmp->quantity = quantity;
			tmp->expiration_date = expiration_date;
			tmp->next = curr;

			if (prev != NULL)
				prev->next = tmp;
			break;
		} else if (curr->expiration_date == expiration_date) {
			curr->quantity += quantity;
			break;
		}
		prev = curr;
		curr = curr->next;
	}
}

void remove_cargo(cargo_hold *cargo, int quantity, int expiration_date)
{
	struct cargo_node *curr = cargo->head;
	struct cargo_node *temp = NULL;
	bool_t done = FALSE;
	if (curr->next == NULL) {
		if (expiration_date == curr->expiration_date) {
			if (quantity < curr->quantity) {
				curr->quantity -= quantity;
			} else {
				free(curr);
			}
		}
	} else {
		/* The node to be deleted is the first node. */
		if (expiration_date == curr->expiration_date) {
			if (quantity < curr->quantity) {
				curr->quantity -= quantity;
			} else {
				cargo->head = curr->next;
				delete_node(NULL, curr);
			}
		} else {
			do {
				if (expiration_date == curr->expiration_date) {
					if (quantity < curr->quantity) {
						curr->quantity -= quantity;
					} else {
						delete_node(temp, curr);
					}
					done = TRUE;
				}
				temp = curr;
				curr = curr->next;
			} while (curr != NULL && !done);
		}
	}
}

void delete_node(struct cargo_node *prev, struct cargo_node *curr)
{
	if (prev != NULL)
		prev->next = curr->next;
	free(curr);
}

int get_cargo_quantity(cargo_hold *cargo)
{
	struct cargo_node *curr;
	int res = 0;

	for (curr = cargo->head; curr != NULL; curr = curr->next) {
		res += curr->quantity;
	}
	return res;
}

void convert_and_sleep(double time_required)
{
	struct timespec sleep_time;

	if (time_required <= 0)
		return;

	/* conversion in timespec to invoke nanosleep() */
	sleep_time.tv_sec = (time_t)time_required;
	sleep_time.tv_nsec =
		(long)((time_required - (double)sleep_time.tv_sec) *
		       NANOSECONDS_IN_SECOND);

	nanosleep(&sleep_time, NULL);
}
