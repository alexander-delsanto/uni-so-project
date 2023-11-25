#define _GNU_SOURCE

#include <stdlib.h>
#include <strings.h>

#include "../lib/shm.h"

#include "include/const.h"
#include "include/utils.h"
#include "include/shm_general.h"
#include "include/shm_offer_demand.h"
#include "include/cargo_list.h"

struct shm_offer {
	int *data;
};

struct shm_demand {
	int *data;
};

shm_offer_t *offer_shm_ports_get(shm_general_t *c);

shm_offer_t *offer_shm_ports_init(shm_general_t *c)
{
	int shm_id;
	size_t size;
	shm_offer_t *offer;

	size = (sizeof(shm_offer_t) + sizeof(int) * get_merci(c)) *
	       get_porti(c);

	shm_id = shm_create(SHM_DATA_PORT_OFFER_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	offer = shm_attach(shm_id);
	bzero(offer, size);
	set_offer_shm_id(c, shm_id);

	return offer;
}

shm_offer_t *offer_shm_ports_get(shm_general_t *c)
{
	shm_offer_t *offer;

	offer = malloc(sizeof(shm_offer_t));
	if (offer == NULL) {
		return NULL;
	}

	offer->data = calloc(get_merci(c), sizeof(int));
	if (offer->data == NULL) {
		return NULL;
	}

	return offer;
}

shm_offer_t *offer_shm_ships_init(shm_general_t *c)
{
	int shm_id;
	size_t size;
	shm_offer_t *offer;

	size = (sizeof(shm_offer_t) + sizeof(int) * get_merci(c)) * get_navi(c);

	shm_id = shm_create(SHM_DATA_SHIP_OFFER_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	offer = shm_attach(shm_id);
	bzero(offer, size);
	set_offer_shm_id(c, shm_id);

	return offer;
}

shm_demand_t *demand_shm_init(shm_general_t *c)
{
	int shm_id;
	size_t size;
	shm_demand_t *demand;

	size = (sizeof(shm_demand_t) + sizeof(int) * get_merci(c)) *
	       get_porti(c);

	shm_id = shm_create(SHM_DATA_DEMAND_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	demand = shm_attach(shm_id);
	bzero(demand, size);
	set_demand_shm_id(c, shm_id);

	return demand;
}

void offer_demand_shm_generate(shm_offer_t *o, shm_demand_t *d, o_list_t *l,
			       int id, shm_general_t *c)
{
	int random_quantity, random_id, random_exp;
	int n_merci, min_life, max_life, size, current_fill;

	if (o == NULL || d == NULL || l == NULL || c == NULL) {
		return;
	}

	current_fill = get_fill(c) / get_days(c);
	size = get_size(c);
	n_merci = get_merci(c);
	min_life = get_min_vita(c);
	max_life = get_max_vita(c);

	while (current_fill > 0) {
		random_id = RANDOM_INTEGER(0, n_merci - 1);
		random_quantity = RANDOM_INTEGER(1, size) % current_fill;

		if (d->data[random_id] > 0) {
			d[id].data[random_id] += random_quantity;
		} else if (o->data[random_id] > 0) {
			o[id].data[random_id] += random_quantity;
			random_exp = RANDOM_INTEGER(min_life, max_life);
			cargo_list_add(l, random_id, random_quantity,
				       random_exp + get_current_day(c));

		} else {
			if (RANDOM_BOOL() == TRUE) {
				o[id].data[random_id] = random_quantity;
				random_exp = RANDOM_INTEGER(min_life, max_life);
				cargo_list_add(l, random_id, random_quantity,
					       random_exp + get_current_day(c));
			} else {
				d[id].data[random_id] = random_quantity;
			}
		}

		current_fill -= random_quantity;
	}
}

void offer_shm_add(shm_offer_t *o, int id, int type, int quantity)
{
	if (o == NULL || quantity == 0) {
		return;
	}

	o[id].data[type] += quantity;
}

void demand_shm_set(shm_demand_t *d, int id, int type, int quantity)
{
	if (d == NULL || quantity == 0) {
		return;
	}

	d[id].data[type] += quantity;
}
void offer_shm_remove(shm_offer_t *o, int id, int type, int quantity)
{
	if (o == NULL || quantity == 0) {
		return;
	}

	o[id].data[type] -= quantity;
}

void demand_shm_remove(shm_demand_t *d, int id, int type, int quantity)
{
	if (d == NULL || quantity == 0) {
		return;
	}

	d[id].data[type] -= quantity;
}

void offer_shm_merge(shm_offer_t *src, shm_offer_t *merge, shm_general_t *c,
		     int id)
{
	int n, i;

	n = get_merci(c);

	for (i = 0; i < n; i++) {
		src[id].data[i] += merge->data[i];
	}
}

shm_offer_t *offer_shm_get_order(shm_offer_t *o, shm_general_t *c, int id,
				 int capacity)
{
	int i, n_merci, cnt;
	shm_offer_t *output;

	cnt = 0;
	n_merci = get_merci(c);
	output = offer_shm_ports_get(c);

	for (i = 0; i < n_merci || cnt == capacity; i++) {
		if (o[id].data[i] == 0) {
			continue;
		}

		if (o[id].data[i] <= capacity - cnt) {
			output->data[i] = o[id].data[i];
			cnt += o[id].data[i];
			o[id].data[i] = 0;
		} else {
			output->data[i] = capacity - cnt;
			o[id].data[i] -= capacity - cnt;
			cnt = capacity;
		}
	}

	return output;
}

o_list_t *offer_shm_get_order_expires(o_list_t *src, shm_offer_t *o,
				      shm_general_t *c)
{
	o_list_t *output;
	int i;

	output = cargo_list_create(c);
	for (i = 0; i < get_merci(c); i++) {
		cargo_list_merge(output,
				 cargo_list_pop_needed(src, c, i, o->data[i]),
				 c);
	}

	return output;
}

void offer_demand_shm_transaction(shm_offer_t *o, shm_demand_t *d, int id_ship,
				  int id_port, shm_general_t *c)
{
	int i, n_merci;

	shm_offer_t *ship;
	shm_demand_t *port;

	ship = &o[id_ship];
	port = &d[id_port];

	n_merci = get_merci(c);

	for (i = 0; i < n_merci; i++) {
		if (port->data[i] == 0) {
			continue;
		}

		if (ship->data[i] >= port->data[i]) {
			ship->data[i] -= port->data[i];
			port->data[i] = 0;
		} else {
			port->data[i] -= ship->data[i];
			ship->data[i] = 0;
		}
	}
}
