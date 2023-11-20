#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#include "include/const.h"
#include "include/shm_config.h"
#include "include/shm_port.h"
#include "include/shm_ship.h"
#include "include/utils.h"

#define GET_DISTANCE(dest)\
	(sqrt(pow(dest.x - ship_shm_get_coords(state.ship, state.id).x, 2) + pow(dest.y - ship_shm_get_coords(state.ship, state.id).y, 2)))

void signal_handler(int signal);

void init_location(void);
void pick_first_destination_port();
void move(struct coord destination_port);

void close_all(void);
void loop(void);
void find_new_destination(int *port_id, struct coord *coords);
void trade(int id_port);

struct state {
	int id;
	shm_config_t *config;
	shm_port_t *port;
	shm_ship_t *ship;
};

struct state state;

int main(int argc, char *argv[])
{
	struct sigaction sa;
	sigset_t mask;
	bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_handler;

	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);

	state.id = (int)strtol(argv[1], NULL, 10);
	state.config = config_shm_attach();
	state.port = port_shm_attach(state.config);
	state.ship = ship_shm_attach(state.config);
	srand(time(NULL) * getpid());
	init_location();

	sigemptyset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGSTORM, &sa, NULL);
	sigaction(SIGMAELSTROM, &sa, NULL);

	pick_first_destination_port();
	while (1) {
		sleep(1);
	}

	return EXIT_SUCCESS;
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
	coords.x = RANDOM_DOUBLE(0, get_lato(state.config));
	coords.y = RANDOM_DOUBLE(0, get_lato(state.config));

	ship_shm_set_coords(state.ship, state.id, coords);
	ship_shm_set_is_moving(state.ship, state.id, TRUE);
}

void pick_first_destination_port()
{
	struct coord dest;
	dest = port_shm_get_coords(state.port, RANDOM_INTEGER(0, get_porti(state.config) - 1));
	move(dest);
}

/**
 * @brief simulates the movement of the ship and updates the location.
 */
void move(struct coord destination_port)
{
	double distance;
	double time_required;
	ship_shm_set_is_moving(state.ship, state.id, TRUE);
	/* calculate distance between actual position and destination */
	distance = GET_DISTANCE(destination_port);
	/* calculate time required to arrive (in days) */
	time_required = distance / get_speed(state.config);
	convert_and_sleep(time_required);
	/* set new location */
	ship_shm_set_coords(state.ship, state.id, destination_port);
	ship_shm_set_is_moving(state.ship, state.id, FALSE);
}

void find_new_destination(int *port_id, struct coord *coords)
{
	/* TODO */
}

void trade(int id_port)
{
	/* TODO */
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
		dprintf(1, "Ship %d: Received SIGDAY signal. Current day: %d\n",
			state.id, get_current_day(state.config));
		/* TODO */
		break;
	case SIGSTORM:
		dprintf(1, "Ship %d: Received SIGSTORM signal.\n", state.id);
		/* TODO */
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
		close_all();
	}
}

void close_all(void)
{
	port_shm_detach(state.port);
	ship_shm_detach(state.ship);
	config_shm_detach(state.config);
	exit(0);
}
