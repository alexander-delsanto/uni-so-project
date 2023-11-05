#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "header/shared_memory.h"
#include "../lib/shm.h"
#include "header/ipc_utils.h"

struct data_ship *ship;
int this_shm_id;

void init_location();
/*void signal_handler(int signal);*/
void move();
void convert_and_sleep(double param, int speed);

int main()
{
	//shm_id_t this_shm_id;
	int cargo_quantity = 0;	/* counter for total objects in cargo
 * 					must be in [0,so_capacity] */

	initialize_shm();
	this_shm_id = get_ship_shm_id();
	
	init_location();

	
	//my_cargo = generate_cargo();

	fprintf(stdout, "\nprima: coord = (%f,%f)\n", ship->coord.x, ship->coord.y);
	
	/* try to move */
	move();
	fprintf(stdout, "\ndopo: coord = (%f,%f)\n", ship->coord.x, ship->coord.y);

    return 0;
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
	set_ship_coords(this_shm_id, ship->coord);
}

void signal_handler(int signal)
{
         /* TODO */
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
	set_ship_coords(this_shm_id, ship->coord);
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