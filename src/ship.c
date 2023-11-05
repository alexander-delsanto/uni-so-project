#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "header/shared_memory.h"
#include "../lib/shm.h"
#include "header/ipc_utils.h"
#include "header/utils.h"

cargo_hold *ship_cargo;
struct data_ship *ship;
struct coordinates *dest;
int _this_id;

void init_location();
void move();
void convert_and_sleep(double param, int speed);
void signal_handler(int signal);
void close_all();

void loop_test();

int main(int argc, char *argv)
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

	_this_id = atoi(&argv[1]);
	//initialize_shm();
	//ship_cargo = calloc(SO_MERCI, sizeof(*ship_cargo));

	//init_location();

	sigemptyset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGSTORM, &sa, NULL);
	sigaction(SIGMAELSTROM, &sa, NULL);


	loop_test();
	//my_cargo = generate_cargo();

	fprintf(stdout, "Prima: coord = (%f,%f)\n", ship->coord.x, ship->coord.y);
	
	/* try to move */
	move();
	fprintf(stdout, "Dopo: coord = (%f,%f)\n", ship->coord.x, ship->coord.y);

    return 0;
}


void loop_test() {
    dprintf(1, "My pid is: %d\n", getpid());
    while (1) {
	    sleep(1);
    }
}


/**
 * @brief initializes ship's location.
 */
void init_location()
{
        /* generate a random location on the map */
	srand((unsigned int)time(NULL) * getpid());
	ship->coord.x = RANDOM_DOUBLE(0, SO_LATO);
	ship->coord.y = RANDOM_DOUBLE(0, SO_LATO);
	set_ship_coords(_this_id, ship->coord);
}

/**
 * @brief simulate the movement of the ship and updates the location.
 */
void move()
{
        double distance;
        /* calculate distance between actual position and destination */
        distance = sqrt(pow(dest->x - ship->coord.x, 2) + pow(dest->y - ship->coord.y, 2));
	convert_and_sleep(distance, SO_SPEED);
        /* new location */
        ship->coord = *dest;
	set_ship_coords(_this_id, ship->coord);
}

/**
 * @brief converts time data from days to nanoseconds and calls nanosleep().
 *
 * @param param data of the action
 * @param speed speed required to perform the action
 */
void convert_and_sleep(double param, int speed)
{
	double time_required;
	struct timespec sleep_time;
	
	/* calculate time required to terminate the operation (in days) */
	time_required = param / speed;
	/* conversion in seconds */
	time_required *= SECONDS_IN_DAY;
	
	/* conversion in timespec to invoke nanosleep() */
	sleep_time.tv_sec = (int) time_required;
	sleep_time.tv_nsec = (long)(time_required - (double)sleep_time.tv_sec) * NANOSECONDS_IN_SECOND;
	
	nanosleep(&sleep_time, NULL);
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
	    dprintf(1, "Received SIGDAY signal.\n");
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
		//dprintf(2, "ship.c: id: %d: Segmentation fault. Terminating.\n", _this_id);
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

}
