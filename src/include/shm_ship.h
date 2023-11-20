#ifndef OS_PROJECT_SHM_SHIP_H
#define OS_PROJECT_SHM_SHIP_H

#include "shm_config.h"

typedef struct shm_ship shm_ship_t;

shm_ship_t *ship_initialize(shm_config_t *c);
shm_ship_t *ship_shm_attach(shm_config_t *c);
void ship_shm_detach(shm_ship_t *c);
void ship_shm_delete(shm_config_t *c);

int ship_shm_get_random_kill(shm_ship_t *s, shm_config_t *c);
int ship_shm_get_random_maelstrom(shm_ship_t *s, shm_config_t *c);

void ship_shm_send_signal_to_all_ships(shm_ship_t *s, shm_config_t *c,
				       int signal);
void ship_shm_send_signal_to_ship(shm_ship_t *s, int id, int signal);

void ship_shm_set_pid(shm_ship_t *s, int id, pid_t pid);
void ship_shm_set_coordinates(shm_ship_t *s, int id, struct coord coord);
void ship_shm_set_dead(shm_ship_t *s, int id);
void ship_shm_set_is_moving(shm_ship_t *s, int id, bool_t value);

#endif
