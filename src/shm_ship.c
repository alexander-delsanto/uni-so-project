#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "../lib/shm.h"

#include "include/const.h"
#include "include/shm_general.h"
#include "include/types.h"
#include "include/shm_ship.h"
#include "include/utils.h"

struct shm_ship {
	pid_t pid;

	bool_t is_dead;
	bool_t is_moving;
	struct coord coords;

	int dump_cargo_available;
	int dump_cargo_shipped;
	int dump_cargo_received;
	int dump_used_docks;
	int dump_ships_arrived;
	bool_t dump_had_swell;
};

shm_ship_t *ship_initialize(shm_general_t *c)
{
	shm_ship_t *ships, *tmp_ships;
	int i;
	int n_ships, id;

	n_ships = get_navi(c);
	id = shm_create(SHM_DATA_SHIPS_KEY, (sizeof(shm_ship_t) * n_ships));
	if (id == -1) {
		dprintf(1, "ciao\n");
		return NULL;
	}
	for (i = 0; i < n_ships; i++) {
		ships[i].is_dead = FALSE;
		ships[i].is_moving = FALSE;
	}

	ships = shm_attach(id);
	set_ship_shm_id(c, id);

	return ships;
}

shm_ship_t *ship_shm_attach(shm_general_t *c)
{
	shm_ship_t *ships;

	ships = shm_attach(get_ship_shm_id(c));

	return ships;
}

void ship_shm_delete(shm_general_t *c)
{
	shm_delete(get_ship_shm_id(c));
}

int ship_shm_get_random_kill(shm_ship_t *s, shm_general_t *c)
{
	int i, n_ships, id = -1;
	bool_t dead;

	n_ships = get_navi(c);

	while (id == -1) {
		for (i = 0; i < n_ships; i++, dead = RANDOM_BOOL()) {
			if (s[i].is_dead == FALSE && s[i].is_moving == TRUE &&
			    dead == TRUE) {
				id = i;
				break;
			}
		}
	}

	return id;
}

int ship_shm_get_random_maelstrom(shm_ship_t *s, shm_general_t *c)
{
	int i, n_ships, id = -1;
	bool_t dead;

	n_ships = get_navi(c);

	while (id == -1) {
		for (i = 0; i < n_ships; i++, dead = RANDOM_BOOL()) {
			if (s[i].is_dead == FALSE && dead == TRUE) {
				id = i;
				break;
			}
		}
	}

	return id;
}

void ship_shm_set_pid(shm_ship_t *s, int id, pid_t pid)
{
	s[id].pid = pid;
}

void ship_shm_set_coords(shm_ship_t *s, int id, struct coord coords)
{
	s[id].coords = coords;
}

void ship_shm_set_dead(shm_ship_t *s, int id)
{
	s[id].is_dead = TRUE;
}


void ship_shm_set_is_moving(shm_ship_t *s, int id, bool_t value)
{
	s[id].is_moving = value;
}


void ship_shm_send_signal_to_all_ships(shm_ship_t *s, shm_general_t *c,
				       int signal)
{
	int i;
	int n_ships;

	for (i = 0; i < n_ships; i++) {
		if (s[i].is_dead == FALSE) {
			kill(s[i].pid, signal);
		}
	}
}

void ship_shm_send_signal_to_ship(shm_ship_t *s, int id, int signal)
{
	kill(s[id].pid, signal);
}

struct coord ship_shm_get_coords(shm_ship_t *s, int id)
{
	return s[id].coords;
}

void ship_shm_detach(shm_ship_t *c)
{
	shm_detach(c);
}
