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
	struct node *node, *prev, *cur;

	if (list == NULL) {
		return;
	}

	prev = NULL;
	cur = list->head;

	while (1) {
		/* If list is empty or expiration date is lower than current element */
		if (cur == NULL || cur->expire > expire) {
			node = create_node(quantity, expire);
			if (node == NULL) {
				return;
			}
			node->next = cur;
			if (prev != NULL) {
				prev->next = node;
			} else {
				list->head = node;
			}

			break;
		} else if (cur->expire == expire) {
			cur->quantity += quantity;
			break;
		}
		prev = cur;
		cur = cur->next;
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

	while (list->head != NULL && cnt > 0) {
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

	if (list->head == NULL && cnt > 0) {
		cargo_list_delete(output);
		output = NULL;
	}

	return output;
}

void cargo_list_delete(o_list_t *list)
{
	struct node *cur, *tmp;

	if (list == NULL) return;

	cur = list->head;
	while (cur != NULL) {
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	free(list);
}

void cargo_list_print_all(o_list_t *list)
{
	struct node *cur;

	cur = list->head;

	while (cur != NULL) {
		dprintf(1, "quantity:%d, expire: %d\n", cur->quantity, cur->expire);
		cur = cur->next;
	}
}

void cargo_list_pop(o_list_t *list, int *quantity, int *expire_day) {
	struct node *tmp;

	if (list == NULL || list->head == NULL) {
		*quantity = -1;
		*expire_day = -1;
		return;
	}

	*quantity = list->head->quantity;
	*expire_day = list->head->expire;

	tmp = list->head;
	list->head = list->head->next;
	free(tmp);
}

int cargo_list_get_not_expired_by_day(o_list_t *list, int expire_day) {
	struct node *cur;
	int qty = 0;

	if (list == NULL || list->head == NULL) {
		return -1;
	}

	for (cur = list->head; cur != NULL; cur = cur->next) {
		if (cur->expire > expire_day)
			qty += cur->quantity;
	}

	return qty;
}

int cargo_list_get_quantity(o_list_t *list) {
	struct node *cur;
	int qty = 0;

	if (list == NULL || list->head == NULL) {
		return -1;
	}

	cur = list->head;

	while (cur != NULL) {
		qty += cur->quantity;
		cur = cur->next;
	}

	return qty;
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
