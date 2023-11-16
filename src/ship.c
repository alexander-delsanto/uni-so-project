#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "header/ipc_utils.h"
#include "header/shared_memory.h"
#include "header/utils.h"

cargo_hold *ship_cargo;
struct coordinates dest;
int _this_id;

#define GET_DISTANCE(dest)\
	(sqrt(pow(dest.x - get_ship_coords(_this_id).x, 2) + pow(dest.y - get_ship_coords(_this_id).y, 2)))

void init_location(void);
void move(struct coordinates destination_port);
void signal_handler(int signal);
void close_all(void);
void loop(void);
void find_new_destination(int *port_id, struct coordinates *coords);
void trade(int id_port);

int main(int argc, char *argv[])
{
	struct sigaction sa;
	sigset_t mask;
	bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_handler;

	/* Signal handler initialization */
	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);

	_this_id = atoi(argv[1]);
	attach_process_to_shm(); /* Still segmentation fault here */
	/*ship_cargo = calloc(SO_MERCI, sizeof(*ship_cargo));*/

	set_ship_is_moving(_this_id, TRUE);
	set_ship_is_dead(_this_id, FALSE);

	/*init_location();*/

	sigemptyset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGSTORM, &sa, NULL);
	sigaction(SIGMAELSTROM, &sa, NULL);





	while(1) {
		sleep(1);
	}
    return 0;
}

void loop(void) {
    int id_dest_port;
    struct coordinates destination_coords;
    while(1) {
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
    	struct coordinates coords;
        /* generate a random location on the map */
	srand(time(NULL) * getpid());
	coords.x = RANDOM_DOUBLE(0, SO_LATO);
	coords.y = RANDOM_DOUBLE(0, SO_LATO);
	set_ship_coords(_this_id, coords);
	set_ship_is_moving(_this_id, FALSE);
	set_ship_is_dead(_this_id, FALSE);
}

/**
 * @brief simulates the movement of the ship and updates the location.
 */
void move(struct coordinates destination_port)
{
        double distance;
	double time_required;
	set_ship_is_moving(_this_id, TRUE);
        /* calculate distance between actual position and destination */
        distance = GET_DISTANCE(dest);
	/* calculate time required to arrive (in days) */
	time_required = distance / SO_SPEED;
	convert_and_sleep(time_required);
        /* set new location */
	set_ship_coords(_this_id, destination_port);
	set_ship_is_moving(_this_id, FALSE);
}

void find_new_destination(int *port_id, struct coordinates *coords)
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
	    dprintf(1, "Ship %d: Received SIGDAY signal. Current day: %d\n", _this_id, get_current_day());
		/* TODO */
		break;
	case SIGSTORM:
		dprintf(1, "Ship %d: Received SIGSTORM signal.\n", _this_id);
		/* TODO */
		break;
	case SIGMAELSTROM:
		dprintf(1, "Ship %d: Received SIGMAELSTROM signal.\n", _this_id);
		set_ship_is_dead(_this_id, TRUE);
		close_all();
	case SIGSEGV:
		dprintf(1, "Received SIGSEGV signal.\n");
		dprintf(2, "ship.c: id: %d: Segmentation fault. Terminating.\n", _this_id);
	case SIGINT:
		close_all();
	}
}

void close_all(void)
{
	detach_all_shm();
	exit(0);
}
