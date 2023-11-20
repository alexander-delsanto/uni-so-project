#define _GNU_SOURCE

#include <stdlib.h>

#include "include/shm_config.h"
#include "include/cargo.h"
#include "include/utils.h"

struct cargo {
	int *offer;
	int *demand;
};

cargo_t *cargo_initialize(shm_config_t *c)
{
	cargo_t *cargo;

	cargo = malloc(sizeof(cargo_t));
	if (cargo == NULL) {
		return NULL;
	}

	cargo->offer = calloc(get_merci(c), sizeof(int));
	if (cargo->offer == NULL) {
		free(cargo);
		return NULL;
	}
	cargo->demand = calloc(get_merci(c), sizeof(int));
	if (cargo->demand == NULL) {
		free(cargo->demand);
		free(cargo);
		return NULL;
	}

	return cargo;
}

void cargo_generate(shm_config_t *c, cargo_t *cargo)
{
	/* TODO rivedere algoritmo
	 * TODO ritornare array di scadenze tipizzato
	 */
	int i, r, tmp, size;
	int cur_fill;

	bool_t pick, offer;

	cur_fill = get_fill(c) / get_days(c);
	size = get_size(c);

	while (cur_fill > 0) {
		for (i = 0; i < get_size(c); i++) {
			if (cargo->demand[i] > 0) {
				continue;
			}

			pick = RANDOM_BOOL();
			offer = RANDOM_BOOL();
			if (pick == TRUE && offer == TRUE) {
				tmp = RANDOM_INTEGER(1, size);
				cargo->offer[i] = tmp;
				cargo->demand[i] = 0;
				/*cargo->offer[i].expire =
				RANDOM_INTEGER(life_min, life_max);*/
				cur_fill -= tmp;
			} else if (pick == TRUE && offer == FALSE) {
				tmp = RANDOM_INTEGER(1, size);
				cargo->offer[i] = 0;
				cargo->demand[i] = tmp;
				cur_fill -= tmp;
			} else {
			}
		}
	}
}

size_t cargo_get_size(shm_config_t *c)
{
	return (sizeof(int) * get_merci(c) * 2) + sizeof(cargo_t);
}
