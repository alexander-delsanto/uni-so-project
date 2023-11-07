#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "header/shared_memory.h"
#include "../lib/shm.h"
#include "header/ipc_utils.h"
#include "header/utils.h"

cargo_hold *ship_cargo;
struct coordinates dest;
int _this_id;

#define GET_DISTANCE(dest)\
	(sqrt(pow(dest.x - get_ship_coords(_this_id).x, 2) + pow(dest.y - get_ship_coords(_this_id).y, 2)))

void init_location();
void move(struct coordinates destination_coords);
void signal_handler(int signal);
void close_all();
void loop();
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
	//ship_cargo = calloc(SO_MERCI, sizeof(*ship_cargo));

	//init_location();

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

void loop() {
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
void init_location()
{
    	struct coordinates coords;
        /* generate a random location on the map */
	srand(time(NULL) * getpid());
	coords.x = RANDOM_DOUBLE(0, SO_LATO);
	coords.y = RANDOM_DOUBLE(0, SO_LATO);
	set_ship_coords(_this_id, coords);
}

/**
 * @brief simulates the movement of the ship and updates the location.
 */
void move(struct coordinates dest)
{
        double distance;
	double time_required;
        /* calculate distance between actual position and destination */
        distance = GET_DISTANCE(dest);
	/* calculate time required to arrive (in days) */
	time_required = distance / SO_SPEED;
	convert_and_sleep(time_required);
        /* set new location */
	set_ship_coords(_this_id, dest);
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
	    dprintf(1, "Ship %d: Received SIGDAY signal.\n", _this_id);
		/* TODO */
		break;
	case SIGSTORM:
		dprintf(1, "Received SIGSTORM signal.\n");
		/* TODO */
		break;
	case SIGMAELSTROM:
		dprintf(1, "Received SIGMAELSTROM signal.\n");
		/* TODO */
		close_all();
		break;
	case SIGSEGV:
		dprintf(1, "Received SIGSEGV signal.\n");
		dprintf(2, "ship.c: id: %d: Segmentation fault. Terminating.\n", _this_id);
		close_all();
		break;
	case SIGINT:
		dprintf(1, "Received SIGINT signal.\n");
		close_all();
		break;
	}
}

void close_all()
{
	exit(0);
	/* TODO */
}
