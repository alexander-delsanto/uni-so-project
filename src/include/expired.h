#ifndef OS_PROJECT_EXPIRED_H
#define OS_PROJECT_EXPIRED_H

#include "shm_general.h"
#include "cargo.h"

typedef struct expired expired_t;

expired_t *expired_init(shm_general_t *c);
void expired_new_day(expired_t *exp, shm_general_t *c);

#endif
