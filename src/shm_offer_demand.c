#define _GNU_SOURCE

#include <stdlib.h>
#include <strings.h>

#include "../lib/shm.h"

#include "include/const.h"
#include "include/utils.h"
#include "include/shm_general.h"
#include "include/shm_offer_demand.h"
#include "include/cargo_list.h"
#include <stdio.h>

#define GET_INDEX(port_id,cargo_id,n_cargo) (port_id * n_cargo + cargo_id)

struct shm_offer {
	int data;
};

struct shm_demand {
	int data;
};

shm_offer_t *offer_shm_ports_get(shm_general_t *g);

/* OFFER */
shm_offer_t *offer_shm_ports_init(shm_general_t *g)
{
	int shm_id;
	size_t size;
	shm_offer_t *offer;

	size = (sizeof(struct shm_offer) * get_porti(g) * get_merci(g));

	shm_id = shm_create(SHM_DATA_PORT_OFFER_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	offer = shm_attach(shm_id);
	bzero(offer, size);
	shm_offer_set_id(g, shm_id);

	return offer;
}

shm_offer_t *shm_offer_ports_attach(shm_general_t *g)
{
	shm_offer_t *offer;
	offer = shm_attach(shm_offer_get_id(g));
	return offer;
}

shm_offer_t *offer_shm_ports_get(shm_general_t *g)
{
	shm_offer_t *offer;

	offer = calloc(get_merci(g), sizeof(shm_offer_t));
	if (offer == NULL) {
		return NULL;
	}

	return offer;
}

void offer_shm_set(shm_offer_t *o, shm_general_t *g, int id, int type,
		   int quantity)
{
	if (o == NULL || quantity == 0) {
		return;
	}

	o[GET_INDEX(id, type, get_merci(g))].data += quantity;
}

void offer_shm_remove(shm_offer_t *o, shm_general_t *g, int id, int type,
		      int quantity)
{
	if (o == NULL || quantity == 0) {
		return;
	}

	o[GET_INDEX(id, type, get_merci(g))].data -= quantity;
}


void offer_shm_delete(shm_offer_t *o)
{
	free(o);
}

void offer_shm_merge(shm_offer_t *src, shm_offer_t *merge, shm_general_t *g,
		     int id)
{
	int n, i, index;

	n = get_merci(g);

	for (i = 0; i < n; i++) {
		index = GET_INDEX(id, i, n);
		src[index].data += merge[i].data;
	}
}

shm_offer_t *offer_shm_get_order(shm_offer_t *o, shm_general_t *g, int id,
				 int capacity)
{
	int i, n_merci, cnt, index;
	shm_offer_t *output;

	cnt = 0;
	n_merci = get_merci(g);
	output = offer_shm_ports_get(g);

	for (i = 0; i < n_merci || cnt == capacity; i++) {
		index = GET_INDEX(id, i, n_merci);
		if (o[index].data == 0) {
			continue;
		}

		if (o[index].data <= capacity - cnt) {
			output[i].data = o[index].data;
			cnt += o[index].data;
			o[index].data = 0;
		} else {
			output[i].data = capacity - cnt;
			o[index].data -= capacity - cnt;
			cnt = capacity;
		}
	}

	return output;
}

o_list_t *offer_shm_get_order_expires(o_list_t *src, shm_offer_t *o,
				      shm_general_t *g)
{
	o_list_t *output;
	int i;

	output = cargo_list_create(g);
	for (i = 0; i < get_merci(g); i++) {
		cargo_list_merge(output, cargo_list_pop_needed(src, g, i, o[i].data), g);
	}

	return output;
}

/* DEMAND */
shm_demand_t *demand_shm_init(shm_general_t *g)
{
	int shm_id;
	size_t size;
	shm_demand_t *demand;

	size = (sizeof(struct shm_offer) * get_porti(g) * get_merci(g));

	shm_id = shm_create(SHM_DATA_DEMAND_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	demand = shm_attach(shm_id);
	bzero(demand, size);
	shm_demand_set_id(g, shm_id);

	return demand;
}

void demand_shm_add(shm_demand_t *d, shm_general_t *g, int id, int type,
		    int quantity)
{
	if (d == NULL || quantity == 0) {
		return;
	}

	d[GET_INDEX(id, type, get_merci(g))].data += quantity;
}

void demand_shm_remove(shm_demand_t *d, shm_general_t *g, int id, int type,
		       int quantity)
{
	if (d == NULL || quantity == 0) {
		return;
	}

	d[GET_INDEX(id, type, get_merci(g))].data -= quantity;
}


/* OFFER + DEMAND */
void offer_demand_shm_delete(shm_general_t *g)
{
	shm_delete(shm_offer_get_id(g));
	shm_delete(shm_demand_get_id(g));
}

void offer_demand_shm_generate(shm_offer_t *o, shm_demand_t *d, o_list_t *l,
			       int id, shm_cargo_t *c, shm_general_t *g)
{
	int random_quantity, random_id, expiration;
	int n_merci, size, current_fill, index;

	if (o == NULL || d == NULL || l == NULL || c == NULL) {
		return;
	}

	current_fill = get_fill(g) / get_days(g);
	n_merci = get_merci(g);
	dprintf(1, "port %d: generating %d fill\n", id, current_fill);
	while (current_fill > 0) {
		random_id = RANDOM_INTEGER(0, n_merci - 1);
		size = shm_cargo_get_size(c, random_id);
		expiration = shm_cargo_get_life(c, random_id);
		dprintf(1, "size %d, life %d\n", size, expiration);
		random_quantity = RANDOM_INTEGER(1, size) % current_fill;
		index = GET_INDEX(id, random_id, n_merci);

		if (d[index].data > 0) {
			d[index].data += random_quantity;
		} else if (o[index].data > 0) {
			o[index].data += random_quantity;
			cargo_list_add(l, random_id, random_quantity,
				       expiration + get_current_day(g));

		} else {
			if (RANDOM_BOOL() == TRUE) {
				o[index].data = random_quantity;
				cargo_list_add(l, random_id, random_quantity,
					       expiration + get_current_day(g));
			} else {
				d[index].data = random_quantity;
			}
		}
		dprintf(1, "done\n");
		current_fill -= random_quantity;
	}
}

int shm_offer_get_quantity(shm_general_t *g, shm_offer_t *o, int port_id, int cargo_id)
{
	return o[GET_INDEX(port_id,cargo_id, get_merci(g))].data;
}

int shm_demand_get_quantity(shm_general_t *g, shm_demand_t *d, int port_id, int cargo_id)
{
	return d[GET_INDEX(port_id,cargo_id, get_merci(g))].data;
}
