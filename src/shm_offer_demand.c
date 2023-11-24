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
			/* TODO bogus! ora come ora non sto usando gli array ma l'istanza singola */
			random_exp = RANDOM_INTEGER(min_life, max_life);
			cargo_list_add(l, random_quantity, random_exp);

		} else {
			if (RANDOM_BOOL() == TRUE) {
				o[id].data[random_id] = random_quantity;
				/* TODO bogus! ora come ora non sto usando gli array ma l'istanza singola */
				random_exp = RANDOM_INTEGER(min_life, max_life);
				cargo_list_add(l, random_quantity, random_exp);
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
