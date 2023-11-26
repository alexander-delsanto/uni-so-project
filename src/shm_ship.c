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

	bool_t is_dead;			/* == dump_had_maelstorm */
	bool_t is_moving;
	struct coord coords;

	bool_t dump_with_cargo;
	bool_t dump_on_port;
	bool_t dump_had_storm;
	bool_t dump_mael_logged;	/* for daily maeltrom */
	bool_t dump_storm_final;	/* for final report */
};

shm_ship_t *ship_initialize(shm_general_t *c)
{
	shm_ship_t *ships;
	int n_ships, id;
	size_t size = sizeof(shm_ship_t) * n_ships;

	n_ships = get_navi(c);
	id = shm_create(SHM_DATA_SHIPS_KEY, size);
	if (id == -1) {
		dprintf(1, "ciao\n");
		return NULL;
	}

	ships = shm_attach(id);
	bzero(ships, size);
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

pid_t ship_shm_get_pid(shm_ship_t *s, int id)
{
	return s[id].pid;
}

bool_t ship_shm_get_dead(shm_ship_t *s, int id)
{
	return s[id].is_dead;
}

bool_t ship_shm_get_is_moving(shm_ship_t *s, int id)
{
	return s[id].is_moving;
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
	s[id].dump_on_port = 1 - value;	/* if ship is moving then it's not on port */
}


void ship_shm_send_signal_to_all_ships(shm_ship_t *s, shm_general_t *c,
				       int signal)
{
	int i;
	int n_ships = get_navi(c);

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

void ship_shm_set_dump_with_cargo(shm_ship_t *s, int id, bool_t value)
{
	s[id].dump_with_cargo = value;
}

void ship_shm_set_dump_had_storm(shm_ship_t *s, int id)
{
	s[id].dump_had_storm = TRUE;
}

int ship_shm_get_dump_with_cargo(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++)
		if(s[id].is_dead == FALSE && (s[id].is_moving == TRUE
					       && s[id].dump_with_cargo == TRUE))
			cnt++;
	return cnt;
}

int ship_shm_get_dump_without_cargo(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++)
		if(s[id].is_dead == FALSE && (s[id].is_moving == TRUE
					       && s[id].dump_with_cargo == FALSE))
			cnt++;
	return cnt;
}

int ship_shm_get_dump_on_port(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++)
		if(s[id].is_dead == FALSE && s[id].dump_on_port == TRUE)
			cnt++;
	return cnt;
}

int ship_shm_get_dump_had_storm(shm_ship_t *s, int n_ships)
{
	int id, cnt = 0;
	for(id = 0; id < n_ships; id++)
		if(s[id].is_dead == FALSE && s[id].dump_had_storm == TRUE)
			cnt++;
	return cnt;
}

int ship_shm_get_dump_had_maelstrom(shm_ship_t *s, int n_ships)
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

int ship_shm_get_dump_is_dead(shm_ship_t *s, int n_ships)
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
