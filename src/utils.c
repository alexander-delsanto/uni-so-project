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

struct cargo_node *push_cargo_node(struct cargo_node *next);
void delete_node(struct cargo_node *prev, struct cargo_node *curr);


void insert_cargo(cargo_hold *cargo, int quantity, int expiration_date)
{
	struct cargo_node *curr = cargo->head;
	struct cargo_node *temp;
	bool_t done = FALSE;
	if (curr == NULL) {
		curr = push_cargo_node(NULL);
		curr->quantity = quantity;
		curr->expiration_date = expiration_date;
	} else if (curr->next == NULL) {
		if(expiration_date == curr->expiration_date) {
			curr->quantity += quantity;
		}
		else if(expiration_date > curr->expiration_date){
			curr->next = push_cargo_node(NULL);
			curr->quantity = quantity;
			curr->expiration_date = expiration_date;
		}
		else{
			temp = curr;
			curr = push_cargo_node(temp);
			curr->quantity = quantity;
			curr->expiration_date = expiration_date;
		}
	} else {
		while(curr != NULL && !done) {
			if(expiration_date == curr->expiration_date) {
				curr->quantity += quantity;
				done = TRUE;
			}
			else if(expiration_date > curr->expiration_date){
				curr = curr->next;
			}
			else{
				temp = curr;
				curr = push_cargo_node(temp);
				curr->quantity = quantity;
				curr->expiration_date = expiration_date;
				done = TRUE;
			}
		}

	}

}

struct cargo_node *push_cargo_node(struct cargo_node *next)
{
	struct cargo_node *new_node;
	new_node = malloc(sizeof(*next));
	new_node->next = next;
	return new_node;
}


void remove_cargo(cargo_hold *cargo, int quantity, int expiration_date)
{
	struct cargo_node *curr = cargo->head;
	struct cargo_node *temp = NULL;
	bool_t done = FALSE;
	if(curr->next == NULL) {
		if(expiration_date == curr->expiration_date) {
			if(quantity < curr->quantity) {
				curr->quantity -= quantity;
			} else {
				free(curr);
			}
		}
	}
	else {
		/* The node to be deleted is the first node. */
		if(expiration_date == curr->expiration_date) {
			if (quantity < curr->quantity) {
				curr->quantity -= quantity;
			} else {
				cargo->head = curr->next;
				delete_node(NULL, curr);
			}
		} else {
			do {
				if(expiration_date == curr->expiration_date) {
					if(quantity < curr->quantity) {
						curr->quantity -= quantity;
					} else {
						delete_node(temp, curr);
					}
					done = TRUE;
				}
				temp = curr;
				curr = curr->next;
			} while(curr != NULL && !done);
		}

	}
}

void delete_node(struct cargo_node *prev, struct cargo_node *curr){
	if(prev != NULL)
		prev->next = curr->next;
	free(curr);
}

void convert_and_sleep(double time_required)
{
	struct timespec sleep_time;

	if(time_required <= 0) return;

	/* conversion in timespec to invoke nanosleep() */
	sleep_time.tv_sec = (time_t) time_required;
	sleep_time.tv_nsec = (long)((time_required - (double)sleep_time.tv_sec) * NANOSECONDS_IN_SECOND);

	nanosleep(&sleep_time, NULL);
}
