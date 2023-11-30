#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

#include "../lib/shm.h"

#include "include/const.h"
#include "include/shm_general.h"
#include "include/types.h"
#include "include/shm_ship.h"
#include "include/utils.h"

struct shm_ship {
	pid_t pid;
	int capacity;

	bool_t is_dead;			/* == dump_had_maelstorm */
	bool_t is_moving;
	bool_t is_at_dock;
	struct coord coords;

	bool_t dump_with_cargo;
	bool_t dump_had_storm;
	bool_t dump_mael_logged;	/* for daily maeltrom */
	bool_t dump_storm_final;	/* for final report */
};

shm_ship_t *shm_ship_initialize(shm_general_t *g)
{
	shm_ship_t *ships;
	int i, n_ships, id;
	size_t size;

	n_ships = get_navi(g);
	size = (sizeof(shm_ship_t) + (sizeof(int) * get_merci(g)) * 2) * n_ships;

	id = shm_create(SHM_DATA_SHIPS_KEY, size);
	if (id == -1) {
		dprintf(1, "ciao\n");
		return NULL;
	}

	ships = shm_attach(id);
	bzero(ships, size);
	for (i = 0; i < n_ships; i++) {
		ships[i].capacity = get_capacity(g);
	}
	shm_ship_set_id(g, id);
	return ships;
}

shm_ship_t *shm_ship_attach(shm_general_t *g)
{
	shm_ship_t *ships;
	ships = shm_attach(shm_ship_get_id(g));

	return ships;
}

void shm_ship_detach(shm_ship_t *g) { shm_detach(g); }

void shm_ship_delete(shm_general_t *g) { shm_delete(shm_ship_get_id(g)); }

/* Signals to ships */
void shm_ship_send_signal_to_all_ships(shm_ship_t *s, shm_general_t *g,
				       int signal)
{
	int i;
	int n_ships = get_navi(g);

	for (i = 0; i < n_ships; i++) {
		if (s[i].is_dead == FALSE) {
			kill(s[i].pid, signal);
		}
	}
}

void shm_ship_send_signal_to_ship(shm_ship_t *s, int id, int signal) { kill(s[id].pid, signal); }

/* Setters */
void shm_ship_set_pid(shm_ship_t *s, int id, pid_t pid) { s[id].pid = pid; }
void shm_ship_set_coords(shm_ship_t *s, int id, struct coord coords) { s[id].coords = coords; }
void shm_ship_set_is_dead(shm_ship_t *s, int id) { s[id].is_dead = TRUE; }
void shm_ship_set_is_moving(shm_ship_t *s, int id, bool_t value){s[id].is_moving = value;}
void shm_ship_set_is_at_dock(shm_ship_t *s, int id, bool_t value){s[id].is_at_dock = value;}

/* Dump setters */
void shm_ship_set_dump_with_cargo(shm_ship_t *s, int id, bool_t value){s[id].dump_with_cargo = value;}
void shm_ship_set_dump_had_storm(shm_ship_t *s, int id){s[id].dump_had_storm = TRUE;}

/* Getters */
pid_t shm_ship_get_pid(shm_ship_t *s, int id){return s[id].pid;}
bool_t shm_ship_get_is_dead(shm_ship_t *s, int id){return s[id].is_dead;}
bool_t shm_ship_get_is_moving(shm_ship_t *s, int id){return s[id].is_moving;}
bool_t shm_ship_get_is_at_dock(shm_ship_t *s, int id){return s[id].is_at_dock;}
struct coord shm_ship_get_coords(shm_ship_t *s, int id){return s[id].coords;}
int shm_ship_get_capacity(shm_ship_t *s, int id){return s[id].capacity;}

/* Dump getters */
int shm_ship_get_dump_with_cargo(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++)
		if(s[id].is_dead == FALSE && (s[id].is_at_dock == FALSE
					       && s[id].dump_with_cargo == TRUE))
			cnt++;
	return cnt;
}
int shm_ship_get_dump_without_cargo(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++)
		if(s[id].is_dead == FALSE && (s[id].is_at_dock == FALSE
					       && s[id].dump_with_cargo == FALSE))
			cnt++;
	return cnt;
}

int shm_ship_get_dump_at_dock(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++)
		if(s[id].is_dead == FALSE && s[id].is_at_dock == TRUE)
			cnt++;
	return cnt;
}

int shm_ship_get_dump_had_storm(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++)
		if(s[id].is_dead == FALSE && s[id].dump_had_storm == TRUE)
			cnt++;
	return cnt;
}

int shm_ship_get_dump_had_maelstrom(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++){
		if(s[id].is_dead == TRUE && s[id].dump_mael_logged == FALSE)
		{
			s[id].dump_mael_logged = TRUE;
			cnt++;
		}

	}
	return cnt;
}

int shm_ship_get_dump_is_dead(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++){
		if(s[id].is_dead == TRUE)
		{
			cnt++;
		}
	}
	return cnt;
}

int ship_shm_get_dump_storm_final(shm_ship_t *s, int n_ships)
{
	int i, cnt = 0;
	for(i = 0; i < n_ships; i++)
		if(s[i].dump_storm_final == TRUE)
			cnt++;
	return cnt;
}

void shm_ship_update_capacity(shm_ship_t *s, int ship_id, int update_value){s[ship_id].capacity += update_value;}

void shm_ship_remove_expired(shm_general_t *g, shm_ship_t *s, shm_cargo_t *c, o_list_t **cargo_hold, int ship_id)
{
	int i, removed, sem_cargo_id;
	sem_cargo_id = sem_cargo_get_id(g);
	for (i = 0; i < get_merci(g); i++) {
		removed = cargo_list_remove_expired(cargo_hold[i], get_current_day(g));
		if (removed > 0) {
			s[ship_id].capacity -= removed * shm_cargo_get_size(c, i);
			shm_cargo_update_dump_available_on_ship(c, i, -removed, sem_cargo_id);
		}
	}

	/* TODO dump */
}

void shm_ship_remove_cargo_maelstrom(shm_general_t *g, shm_ship_t *s, shm_cargo_t *c, o_list_t **cargo_hold, int ship_id)
{
	int i, to_remove, sem_cargo_id;
	sem_cargo_id = sem_cargo_get_id(g);

	for (i = 0; i < get_merci(g); i++){
		to_remove = cargo_list_get_quantity(cargo_hold[i]);
		shm_cargo_update_dump_available_on_ship(c, i, -to_remove, sem_cargo_id);
	}
}
