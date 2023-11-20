#ifndef OS_PROJECT_EXPIRED_H
#define OS_PROJECT_EXPIRED_H

#include "shm_config.h"
#include "cargo.h"
#include "shm_port.h"

typedef struct expired expired_t;

expired_t *expired_init(shm_config_t *c);
void expired_new_day(expired_t *exp, shm_config_t *c);

#endif
