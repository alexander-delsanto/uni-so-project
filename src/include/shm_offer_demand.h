#ifndef OS_PROJECT_OFFER_H
#define OS_PROJECT_OFFER_H

#include <stdlib.h>
#include "shm_general.h"
#include "types.h"

typedef struct shm_offer shm_offer_t;
typedef struct shm_demand shm_demand_t;

shm_offer_t *offer_shm_init(shm_general_t *c);
shm_demand_t *demand_shm_init(shm_general_t *c);

void offer_demand_shm_generate(shm_offer_t *o, shm_demand_t *d,
			       shm_general_t *c);

void offer_shm_set(shm_offer_t *o, int id, int quantity);
void offer_shm_remove(shm_offer_t *o, int id, int quantity);

void demand_shm_set(shm_demand_t *d, int id, int quantity);
void demand_shm_remove(shm_demand_t *d, int id, int quantity);

#endif
