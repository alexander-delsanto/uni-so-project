#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "../lib/shm.h"
#include "../lib/semaphore.h"

#include "include/utils.h"
#include "include/const.h"
#include "include/shm_general.h"
#include "include/types.h"
#include "include/shm_port.h"
#include "include/shm_cargo.h"

static void shm_cargo_values_init(shm_general_t *g, shm_cargo_t *cargo);

struct shm_cargo {
	int batch_size;
	int batch_life;

	/* for final report */
	int dump_total_generated;
	int dump_stayed_in_port;
	int dump_expired_in_port;
	int dump_expired_on_ship;
	int dump_received_in_port;
	/* for daily report */
	int dump_available_in_port;
	int dump_available_on_ship;
	int dump_daily_expired_on_ship;
	int dump_daily_expired_in_port;
};

shm_cargo_t *shm_cargo_initialize(shm_general_t *g)
{
	shm_cargo_t *cargo;
	int shm_id, n_cargo, i;
	size_t size;

	n_cargo = get_merci(g);

	size = (sizeof(struct shm_cargo) * n_cargo);

	shm_id = shm_create(SHM_DATA_CARGO_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	cargo = shm_attach(shm_id);
	bzero(cargo, size);
	shm_cargo_set_id(g, shm_id);

	shm_cargo_values_init(g, cargo);

	for (i = 0; i < n_cargo; i++) {
		dprintf(1, "cargo %d: size: %d, life: %d\n", i,
			cargo[i].batch_size, cargo[i].batch_life);
	}

	return cargo;
}

static void shm_cargo_values_init(shm_general_t *g, shm_cargo_t *cargo)
{
	int i, n_cargo;

	n_cargo = get_merci(g);
	for (i = 0; i < n_cargo; i++) {
		cargo[i].batch_size = RANDOM_INTEGER(1, get_size(g));
		cargo[i].batch_life = RANDOM_INTEGER(get_min_vita(g), get_max_vita(g));
	}
}

shm_cargo_t *shm_cargo_attach(shm_general_t *g)
{
	shm_cargo_t *cargo;

	cargo = shm_attach(shm_cargo_get_id(g));
	return cargo;
}

void shm_cargo_detach(shm_cargo_t *c)
{
	shm_detach(c);
}

void cargo_shm_delete(shm_general_t *c)
{
	shm_delete(shm_cargo_get_id(c));
}



/* Getters */
int shm_cargo_get_size(shm_cargo_t *c, int id){return c[id].batch_size;}
int shm_cargo_get_life(shm_cargo_t *c, int id){return c[id].batch_life;}

int shm_cargo_get_min_size_id(shm_cargo_t *c, shm_general_t *g)
{
	int i, min, tmp_batch;

	for(i = 0, tmp_batch = 0; i < get_merci(g); i++){
		if(c[i].batch_size < tmp_batch || tmp_batch == 0) {
			min = i;
			tmp_batch = c[i].batch_size;
		}
	}
	return min;
}


int shm_cargo_get_dump_total_generated(shm_cargo_t *c, int id){return c[id].dump_total_generated;}
int shm_cargo_get_dump_stayed_in_port(shm_cargo_t *c, int id){return c[id].dump_stayed_in_port;}
int shm_cargo_get_dump_expired_in_port(shm_cargo_t *c, int id){return c[id].dump_expired_in_port;}
int shm_cargo_get_dump_expired_on_ship(shm_cargo_t *c, int id){return c[id].dump_expired_on_ship;}
int shm_cargo_get_dump_received_in_port(shm_cargo_t *c, int id){return c[id].dump_received_in_port;}
int shm_cargo_get_dump_available_in_port(shm_cargo_t *c, int id){return c[id].dump_available_in_port;}
int shm_cargo_get_dump_available_on_ship(shm_cargo_t *c, int id){return c[id].dump_available_on_ship;}
int shm_cargo_get_dump_daily_expired_in_port(shm_cargo_t *c, int id){return c[id].dump_daily_expired_in_port;}
int shm_cargo_get_dump_daily_expired_on_ship(shm_cargo_t *c, int id){return c[id].dump_daily_expired_on_ship;}

/* Setters */
void shm_cargo_set_dump_total_generated(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_total_generated += quantity;
}
void shm_cargo_set_dump_stayed_in_port(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_stayed_in_port += quantity;
}
void shm_cargo_set_dump_expired_in_port(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_expired_in_port += quantity;
}
void shm_cargo_set_dump_expired_on_ship(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_expired_on_ship += quantity;
}
void shm_cargo_set_dump_received_in_port(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_received_in_port += quantity;
}
void shm_cargo_set_dump_available_in_port(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_available_in_port += quantity;
}
void shm_cargo_set_dump_available_on_ship(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_available_on_ship += quantity;
}

void shm_cargo_set_dump_daily_expired_in_port(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_daily_expired_in_port = quantity;
	c[id].dump_available_in_port -= quantity;
}
void shm_cargo_set_dump_daily_expired_on_ship(shm_cargo_t *c, int id, int quantity)
{
	c[id].dump_daily_expired_on_ship = quantity;
	c[id].dump_available_on_ship -= quantity;
}
