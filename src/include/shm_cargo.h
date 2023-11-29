#ifndef OS_PROJECT_SHM_CARGO_H
#define OS_PROJECT_SHM_CARGO_H

#include <sys/types.h>

#include "shm_general.h"
#include "types.h"

typedef struct shm_cargo shm_cargo_t;

/* Cargo shm */
shm_cargo_t *shm_cargo_initialize(shm_general_t *g);
shm_cargo_t *shm_cargo_attach(shm_general_t *g);
void shm_cargo_detach(shm_cargo_t *c);
void cargo_shm_delete(shm_general_t *c);

/* Getters */
int shm_cargo_get_size(shm_cargo_t *c, int id);
int shm_cargo_get_life(shm_cargo_t *c, int id);
int shm_cargo_get_min_size_id(shm_cargo_t *c, shm_general_t *g);

int shm_cargo_get_dump_total_generated(shm_cargo_t *c, int id);
int shm_cargo_get_dump_stayed_in_port(shm_cargo_t *c, int id);
int shm_cargo_get_dump_expired_in_port(shm_cargo_t *c, int id);
int shm_cargo_get_dump_expired_on_ship(shm_cargo_t *c, int id);
int shm_cargo_get_dump_received_in_port(shm_cargo_t *c, int id);
int shm_cargo_get_dump_available_in_port(shm_cargo_t *c, int id);
int shm_cargo_get_dump_available_on_ship(shm_cargo_t *c, int id);
int shm_cargo_get_dump_daily_expired_in_port(shm_cargo_t *c, int id);
int shm_cargo_get_dump_daily_expired_on_ship(shm_cargo_t *c, int id);

/* Setters */
void shm_cargo_set_dump_total_generated(shm_cargo_t *c, int id, int quantity);
void shm_cargo_set_dump_stayed_in_port(shm_cargo_t *c, int id, int quantity);
void shm_cargo_set_dump_expired_in_port(shm_cargo_t *c, int id, int quantity);
void shm_cargo_set_dump_expired_on_ship(shm_cargo_t *c, int id, int quantity);
void shm_cargo_set_dump_received_in_port(shm_cargo_t *c, int id, int quantity);
void shm_cargo_set_dump_available_in_port(shm_cargo_t *c, int id, int quantity);
void shm_cargo_set_dump_available_on_ship(shm_cargo_t *c, int id, int quantity);
void shm_cargo_set_dump_daily_expired_in_port(shm_cargo_t *c, int id, int quantity);
void shm_cargo_set_dump_daily_expired_on_ship(shm_cargo_t *c, int id, int quantity);


#endif
