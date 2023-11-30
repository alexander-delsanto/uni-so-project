#define _GNU_SOURCE

#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#include "../lib/shm.h"

#include "include/const.h"
#include "include/utils.h"
#include "include/shm_general.h"
#include "include/shm_offer_demand.h"
#include "include/cargo_list.h"

/**
 * @brief Macro to calculate the index in the shared memory array.
 */
#define GET_INDEX(port_id,cargo_id,n_cargo) (port_id * n_cargo + cargo_id)

struct shm_offer {
	int data;
	int dump_tot_offered;
};

struct shm_demand {
	int data;
	int dump_tot_demanded;
};

/* OFFER SHM FUNCTIONS */

shm_offer_t *shm_offer_init(shm_general_t *g)
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

shm_offer_t *shm_offer_attach(shm_general_t *g)
{
	shm_offer_t *offer;
	offer = shm_attach(shm_offer_get_id(g));
	return offer;
}

void shm_offer_detach(shm_offer_t *o)
{
	shm_detach(o);
}

void shm_offer_remove_quantity(shm_offer_t *o, shm_general_t *g, int id, int type,
		      int quantity)
{
	if (o == NULL || quantity == 0) {
		return;
	}

	o[GET_INDEX(id, type, get_merci(g))].data -= quantity;
}

int shm_offer_get_quantity(shm_general_t *g, shm_offer_t *o, int port_id, int cargo_id)
{
	return o[GET_INDEX(port_id,cargo_id, get_merci(g))].data;
}

int shm_offer_get_tot_quantity(shm_general_t *g, shm_offer_t *o, int port_id)
{
	int i, qty = 0;
	int n_merci = get_merci(g);
	for(i = 0; i < n_merci; i++) {
		qty += o[GET_INDEX(port_id, i, n_merci)].data;
	}
	return qty;
}

/* DEMAND SHM FUNCTIONS */

shm_demand_t *shm_demand_init(shm_general_t *g)
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

shm_demand_t *shm_demand_attach(shm_general_t *g)
{
	shm_demand_t *demands;
	demands = shm_attach(shm_demand_get_id(g));
	return demands;
}

void shm_demand_detach(shm_demand_t *d)
{
	shm_detach(d);
}

int shm_demand_get_quantity(shm_general_t *g, shm_demand_t *d, int port_id, int cargo_id)
{
	return d[GET_INDEX(port_id,cargo_id, get_merci(g))].data;
}

void shm_demand_remove_quantity(shm_demand_t *d, shm_general_t *g, int id, int type,
		       int quantity)
{
	if (d == NULL || quantity == 0) {
		return;
	}

	d[GET_INDEX(id, type, get_merci(g))].data -= quantity;
}

/* OFFER AND DEMAND SHM FUNCTIONS */

void shm_offer_demand_delete(shm_general_t *g)
{
	shm_delete(shm_offer_get_id(g));
	shm_delete(shm_demand_get_id(g));
}

void shm_offer_demand_generate(shm_offer_t *o, shm_demand_t *d, o_list_t **l,
			       int port_id, shm_cargo_t *c, shm_general_t *g)
{
	bool_t filled;
	int random_quantity, random_id, expiration;
	int n_merci, size, fill, current_fill, index, i, cur_id;
	int id_min, size_min;
	int sem_cargo_id = sem_cargo_get_id(g);

	if (o == NULL || d == NULL || l == NULL || c == NULL) {
		return;
	}

	fill = get_fill(g) / get_days(g);
	current_fill = fill;
	n_merci = get_merci(g);
	id_min = shm_cargo_get_min_size_id(c, g);
	size_min = shm_cargo_get_size(c, id_min);

	while (current_fill > 0) {
		filled = TRUE;
		random_id = RANDOM_INTEGER(0, n_merci - 1);
		random_quantity = (RANDOM_INTEGER(1, current_fill));
		size = shm_cargo_get_size(c, random_id);

		if ((random_quantity * size) > current_fill) {
			for (i = 0; i < n_merci; i++) {
				cur_id = (random_id + i + 1) % n_merci;
				size = shm_cargo_get_size(c, cur_id);
				random_quantity = (RANDOM_INTEGER(1, current_fill));

				if (((random_quantity * size) >= current_fill && (current_fill >= size))) {
					random_quantity = RANDOM_INTEGER(1, current_fill / size);
					random_id = cur_id;
					filled = TRUE;
					break;
				}
				filled = FALSE;
			}

			if (filled == FALSE) {
				if (size <= current_fill) {
					random_quantity = current_fill / size_min;
					random_id = id_min;
				} else {
					break;
				}
			}
		}

		expiration = shm_cargo_get_life(c, random_id);
		index = GET_INDEX(port_id, random_id, n_merci);

		if (d[index].data > 0) {
			d[index].data += random_quantity;
			d[index].dump_tot_demanded += random_quantity;
		} else if (o[index].data > 0) {
			o[index].data += random_quantity;
			o[index].dump_tot_offered += random_quantity;
			cargo_list_add(l[random_id], random_quantity, expiration + get_current_day(g));
			shm_cargo_update_dump_available_in_port(c, random_id, random_quantity, sem_cargo_id);
			shm_cargo_update_dump_total_generated(c, random_id, random_quantity, sem_cargo_id);
		} else {
			if (RANDOM_BOOL() == TRUE) {
				o[index].data = random_quantity;
				o[index].dump_tot_offered += random_quantity;
				cargo_list_add(l[random_id],random_quantity, expiration + get_current_day(g));
				shm_cargo_update_dump_available_in_port(c, random_id, random_quantity, sem_cargo_id);
				shm_cargo_update_dump_total_generated(c, random_id, random_quantity, sem_cargo_id);
			} else {
				d[index].dump_tot_demanded += random_quantity;
				d[index].data = random_quantity;
			}
		}

		current_fill -= random_quantity * size;
	}
}

int shm_offer_get_dump_highest(shm_general_t *g, shm_offer_t *o, int cargo_type)
{
	int i, n_cargo, tmp, max = -1, max_id = -1;
	n_cargo = get_merci(g);
	for (i = 0; i < get_porti(g); i++) {
		tmp = o[GET_INDEX(i, cargo_type, n_cargo)].dump_tot_offered;
		if (tmp > max) {
			max = tmp;
			max_id = i;
		}
	}
	return max_id;
}

int shm_demand_get_dump_highest(shm_general_t *g, shm_demand_t *d, int cargo_type)
{
	int i, n_cargo, tmp, max = -1, max_id = -1;
	n_cargo = get_merci(g);
	for (i = 0; i < get_porti(g); i++) {
		tmp = d[GET_INDEX(i, cargo_type, n_cargo)].dump_tot_demanded;
		if (tmp > max) {
			max = tmp;
			max_id = i;
		}
	}
	return max_id;
}
