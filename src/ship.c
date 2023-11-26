#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#include "include/const.h"
#include "include/sem.h"
#include "include/shm_general.h"
#include "include/shm_port.h"
#include "include/shm_ship.h"
#include "include/utils.h"
#include "include/shm_offer_demand.h"
#include "include/cargo_list.h"

#define GET_DISTANCE(dest)\
	(sqrt(pow(dest.x - ship_shm_get_coords(state.ship, state.id).x, 2) + pow(dest.y - ship_shm_get_coords(state.ship, state.id).y, 2)))

void signal_handler(int signal);

void init_location(void);
void pick_first_destination_port(void);
void move(struct coord destination_port);

void close_all(void);
void loop(void);
void find_new_destination(int *port_id, struct coord *coords);
void trade(int id_port);

struct state {
	int id;
	shm_general_t *general;
	shm_port_t *port;
	shm_ship_t *ship;

	shm_offer_t *offer;
	o_list_t *cargo;

	int current_day;
};

struct state state;

int main(int argc, char *argv[])
{
	int day;
	struct sigaction sa;
	sigset_t mask;
	bzero(&state, sizeof (struct state));
	bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_handler;

	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);

	state.id = (int)strtol(argv[1], NULL, 10);
	general_shm_attach(&state.general);
	state.port = port_shm_attach(state.general);
	state.ship = ship_shm_attach(state.general);
	state.offer = offer_shm_ships_init(state.general);
	state.cargo = cargo_list_create(state.general);

	srand(time(NULL) * getpid());
	init_location();

	sigemptyset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGSTORM, &sa, NULL);
	sigaction(SIGMAELSTROM, &sa, NULL);

	sem_execute_semop(get_sem_port_init_id(), 0, 0, 0);
	sem_execute_semop(get_sem_start_id(), 0, 0, 0);

	pick_first_destination_port();

	while (1) {
		day = get_current_day(state.general);
		if (state.current_day < day) {
			/* TODO: new day operations */
			state.current_day = day;
		}
	}
}

void loop(void) {
	int id_dest_port;
	struct coord destination_coords;
	while (1) {

		find_new_destination(&id_dest_port, &destination_coords);
		move(destination_coords);
		trade(id_dest_port);
	}
}

/**
 * @brief initializes ship's location.
 */
void init_location(void)
{
	struct coord coords;
	/* generate a random location on the map */
	coords.x = RANDOM_DOUBLE(0, get_lato(state.general));
	coords.y = RANDOM_DOUBLE(0, get_lato(state.general));

	ship_shm_set_coords(state.ship, state.id, coords);
	ship_shm_set_is_moving(state.ship, state.id, TRUE);
	/* TODO
	 * ship_shm_set_dump_with_cargo(state.ship, state.id, FALSE);*/
}

void pick_first_destination_port(void)
{
	struct coord dest;
	dest = port_shm_get_coords(state.port, RANDOM_INTEGER(0, get_porti(state.general) - 1));
	move(dest);
}

/**
 * @brief simulates the movement of the ship and updates the location.
 */
void move(struct coord destination_port)
{
	double distance;
	double time_required;
/*	dprintf(1, "ship %d: before x: %lf y: %lf\n", state.id, ship_shm_get_coords(state.ship, state.id).x, ship_shm_get_coords(state.ship, state.id).y);*/
	ship_shm_set_is_moving(state.ship, state.id, TRUE);
	/* calculate distance between actual position and destination */
	distance = GET_DISTANCE(destination_port);
	/* calculate time required to arrive (in days) */
	time_required = distance / get_speed(state.general);
	convert_and_sleep(time_required);
	/* set new location */
	ship_shm_set_coords(state.ship, state.id, destination_port);
/*	dprintf(1, "ship %d: after x: %lf y: %lf\n", state.id, ship_shm_get_coords(state.ship, state.id).x, ship_shm_get_coords(state.ship, state.id).y);*/
	ship_shm_set_is_moving(state.ship, state.id, FALSE);
}

void find_new_destination(int *port_id, struct coord *coords)
{
	/* TODO */
}

void trade(int id_port)
{
	bool_t is_selling;
	/* TODO */
	is_selling = TRUE;

	if (is_selling == TRUE) {
		/*
		 * Step:
		 * genera offer da inviare al porto
		 * genera relative scadenze
		 *
		 * invia offer e scadenze al porto
		 * il porto farà il merge di offer e scadenze
		 *
		 * nave riparte
		 */
	} else {
		/*
		 * Step:
		 * porto genera carico usando la offer
		 * porto genera relative scadenze
		 *
		 * porto invia offer e scadenze a nave
		 *
		 * nave riceve offer e scadenze
		 * nave fa merge di offer e scadenze
		 *
		 * nave riparte
		 */
	}
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
		break;
	case SIGSTORM:
		dprintf(1, "Ship %d: Received SIGSTORM signal.\n", state.id);
		ship_shm_set_dump_had_storm(state.ship, state.id);
		convert_and_sleep(get_storm_duration(state.general) / 24.0);
		break;
	case SIGMAELSTROM:
		dprintf(1, "Ship %d: Received SIGMAELSTROM signal.\n",
			state.id);
		ship_shm_set_dead(state.ship, state.id);
		close_all();
	case SIGSEGV:
		dprintf(1, "Received SIGSEGV signal.\n");
		dprintf(2, "ship.c: id: %d: Segmentation fault. Terminating.\n",
			state.id);
	case SIGINT:
		dprintf(1, "ship %d: received SIGINT\n", state.id);
		close_all();
	}
}

void close_all(void)
{
	port_shm_detach(state.port);
	ship_shm_detach(state.ship);
	general_shm_detach(state.general);
	exit(0);
}
