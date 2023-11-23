#define _GNU_SOURCE

#include <stdlib.h>
#include <strings.h>

#include "../lib/shm.h"

#include "include/const.h"
#include "include/utils.h"
#include "include/shm_general.h"
#include "include/shm_offer_demand.h"

struct shm_offer {
	int *data;
};

struct shm_demand {
	int *data;
};

shm_offer_t *offer_shm_init(shm_general_t *c)
{
	int shm_id;
	size_t size;
	shm_offer_t *offer;

	size = (sizeof(shm_offer_t) + sizeof(int) * get_merci(c)) *
	       get_porti(c);

	shm_id = shm_create(SHM_DATA_OFFER_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	offer = shm_attach(shm_id);
	bzero(offer, size);
	//set_port_shm_id(g, shm_id); set offer_demand shm_id

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
	//set_port_shm_id(g, shm_id); set offer_demand shm_id

	return demand;
}

void offer_demand_shm_generate(shm_offer_t *o, shm_demand_t *d, int id,
			       shm_general_t *c)
{
	int random_quantity, random_id;
	int n_merci, size, current_fill;

	current_fill = get_fill(c) / get_days(c);
	size = get_size(c);
	n_merci = get_merci(c);

	while (current_fill > 0) {
		random_id = RANDOM_INTEGER(1, n_merci);
		random_quantity = RANDOM_INTEGER(1, size) % current_fill;

		if (d->data[random_id] > 0) {
			d[id].data[random_id] += random_quantity;
		} else if (o->data[random_id] > 0) {
			o[id].data[random_id] += random_quantity;

		} else {
			if (RANDOM_BOOL() == TRUE) {
				o[id].data[random_id] = random_quantity;
			} else {
				d[id].data[random_id] = random_quantity;
			}
		}

		current_fill -= random_quantity;
	}
}

void offer_shm_add(shm_offer_t *o, int id, int type, int quantity)
{
	if (o == NULL) {
		return;
	}

	o[id].data[type] += quantity;
}

void demand_shm_set(shm_demand_t *d, int id, int type, int quantity)
{
	if (d == NULL) {
		return;
	}

	d[id].data[type] += quantity;
}
void offer_shm_remove(shm_offer_t *o, int id, int type, int quantity)
{
	if (o == NULL) {
		return;
	}

	o[id].data[type] -= quantity;
}

void demand_shm_remove(shm_demand_t *d, int id, int type, int quantity)
{
	if (d == NULL) {
		return;
	}

	d[id].data[type] -= quantity;
}
