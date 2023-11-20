#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header/cargo.h"
#include "header/utils.h"

struct goods {
	int quantity;
	int expire;
};

struct cargo {
	int size;
	int fill;
	int *demand;
	goods_t *offer;
};

cargo_t *cargo_generate(int ids, int fill)
{
	cargo_t *cargo;

	cargo = malloc(sizeof(cargo_t) * ids);
	if (cargo == NULL) {
		return NULL;
	}

	cargo->size = ids;
	/*
	 * TODO: il fill viene settato dal porto, la cui somma totale è SO_FILL
	 * Penso che il master debba occuparsi di generare questi numeri e comunicarlo al porto
	 */
	cargo->fill = fill;
	cargo->demand = calloc(cargo->size, sizeof(int));
	if (cargo->demand == NULL) {
		return NULL;
	}
	cargo->offer = malloc(sizeof(goods_t) * cargo->size);
	if (cargo->offer == NULL) {
		return NULL;
	}

	return cargo;
}

void cargo_generate_goodies(cargo_t *cargo, int size, int life_min,
			    int life_max)
{
	int i, r, tmp;
	int cur_fill;

	cur_fill = cargo->fill;

	srand(time(NULL));
	for (i = 0; i < cargo->size; i++) {
		r = rand() % 2;
		if (r == 1) {
			tmp = RANDOM_INTEGER(1, size);
			cargo->offer[i].quantity = tmp;
			cargo->offer[i].expire =
				RANDOM_INTEGER(life_min, life_max);
			cur_fill -= tmp;
		} else {
			cargo->offer[i].quantity = 0;
			cargo->offer[i].expire = 0;
			cargo->demand[i] = 100;
			cur_fill -= 100;
		}
	}
}

void cargo_update_expire(cargo_t *cargo)
{
	int i;

	if (cargo == NULL) {
		return;
	}

	for (i = 0; i < cargo->size; i++) {
		if (cargo->offer[i].quantity > 0) {
			cargo->offer[i].expire--;
			if (cargo->offer[i].expire <= 0) {
				cargo->offer[i].quantity = 0;
			}
		}
	}
}

void cargo_delete(cargo_t *cargo)
{
	if (cargo == NULL) {
		return;
	}

	free(cargo->demand);
	free(cargo->offer);
	free(cargo);
}
