#include <stdlib.h>
#include <stdio.h>

#include "include/cargo_list.h"

struct node {
	int quantity;
	int expire;
	struct node *next;
};

struct o_list {
	struct node *head;
};

static struct node *create_node(int quantity, int expire);

o_list_t *cargo_list_create(void)
{
	o_list_t *list;

	list = calloc(1, sizeof(o_list_t));
	if (list == NULL) {
		return NULL;
	}
	list->head = NULL;

	return list;
}

void cargo_list_add(o_list_t *list, int quantity, int expire)
{
	struct node *node, *prev, *curr;

	if (list == NULL) {
		return;
	}

	prev = NULL;
	curr = list->head;

	while (1) {
		/* If list is empty or expiration date is lower than currrent element */
		if (curr == NULL || curr->expire > expire) {
			node = create_node(quantity, expire);
			if (node == NULL) {
				return;
			}
			node->next = curr;
			if (prev != NULL) {
				prev->next = node;
			} else {
				list->head = node;
			}

			break;
		} else if (curr->expire == expire) {
			curr->quantity += quantity;
			break;
		}
		prev = curr;
		curr = curr->next;
	}
}

int cargo_list_remove_expired(o_list_t *list, int expire_day)
{
	struct node *tmp;
	int qt;

	if (list == NULL) {
		return -1;
	}

	if (list->head == NULL) {
		return 0;
	}

	qt = 0;

	if (list->head->expire == expire_day) {
		qt = list->head->quantity;
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}

	return qt;
}

o_list_t *cargo_list_pop_needed(o_list_t *list, int quantity)
{
	o_list_t *output;

	struct node *tmp;
	int cnt;

	if (list == NULL) {
		return NULL;
	}

	if (list->head == NULL || quantity == 0) {
		return NULL;
	}

	output = cargo_list_create();
	if (output == NULL) {
		return NULL;
	}

	cnt = quantity;

	while (list->head->next != NULL || cnt > 0) {
		if (list->head->quantity <= cnt) {
			cargo_list_add(output, list->head->quantity, list->head->expire);
			cnt -= list->head->quantity;
			tmp = list->head;
			list->head = list->head->next;
			free(tmp);
		} else {
			cargo_list_add(output, cnt, list->head->expire);
			list->head->quantity -= cnt;
			break;
		}
	}

	return output;
}

void cargo_list_delete(o_list_t *list)
{
	struct node *curr, *tmp;

	curr = list->head;

	while (curr != NULL) {
		tmp = curr;
		curr = curr->next;
		free(tmp);
	}

	free(list);
}

void cargo_list_print_all(o_list_t *list)
{
	struct node *curr;

	curr = list->head;

	while (curr != NULL) {
		printf("%d %d\n", curr->quantity, curr->expire);
		curr = curr->next;
	}
}

static struct node *create_node(int quantity, int expire)
{
	struct node *node;

	node = malloc(sizeof(struct node));
	if (node == NULL) {
		return NULL;
	}

	node->quantity = quantity;
	node->expire = expire;
	node->next = NULL;

	return node;
}
