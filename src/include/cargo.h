#ifndef OS_PROJECT_CARGO_H
#define OS_PROJECT_CARGO_H

#include <stdlib.h>

#include "shm_config.h"
#include "expired.h"

typedef struct cargo cargo_t;

typedef int *cargo_exp_t;

cargo_t *cargo_initialize(shm_config_t *c);
size_t cargo_get_size(shm_config_t *c);
void cargo_generate(shm_config_t *c, cargo_t *cargo);

#endif
