#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "header/shared_memory.h"
#include "../lib/shm.h"

#define NANOSECONDS_IN_SECOND 1000000000L /* constant for conversion to ns */
#define SECONDS_IN_DAY 86400 /* seconds in a day */

int base = 100; // example base of the map
//int capacity = 1000; // example capacity
int speed = 10; // example speed

struct data_ship *ship;
struct coordinates *dest;

shm *ship_shm;

#define SIZE (sizeof(*ship))

void init_location();
/*void signal_handler(int signal);*/
void move();
void convert_and_sleep(double param, int speed);	/* we need it for move() and mooring in port */
struct data_cargo * generate_cargo();

int main()
{
	//shm_id_t this_id;
	int cargo_quantity = 0;	/* counter for total objects in cargo
 * 					must be in [0,so_capacity] */
	struct data_cargo *my_cargo = NULL;
	/* init signals */

	/* obtain the necessary data from data_general
	 * (so_speed, so_capacity, so_lato)
	 */
	
	/* example */
	dest = malloc(sizeof(*dest));
	dest->x = 6.0;
	dest->y = 8.0;
	ship = malloc(SIZE);

	ship_shm = shm_create(SHM_DATA_SHIPS_KEY, SIZE);
	
	init_location();
	
	my_cargo = generate_cargo();
	
	ship = shm_read(ship_shm);
	fprintf(stdout, "\nprima: coord = (%f,%f)\n", ship->coord.x, ship->coord.y);
	
	/* try to move */
	move();
	shm_write(ship_shm, (void*) ship, SIZE);
	ship = shm_read(ship_shm);
	fprintf(stdout, "\ndopo: coord = (%f,%f)\n", ship->coord.x, ship->coord.y);

	free(ship);
	shm_delete(ship_shm);

        return 0;
}

void init_location()
{
        /* generate a random location on the map */
	srand((unsigned int)time(NULL) * getpid());
	ship->coord.x = rand() % base;
	ship->coord.y = rand() % base;
}

void signal_handler(int signal)
{
         /* TODO */
}

void move()
{
        double distance;
        /* calculate distance between actual position and destination */
        distance = sqrt(pow(dest->x - ship->coord.x, 2) + pow(dest->y - ship->coord.y, 2));
	/* conversions and wait */
	convert_and_sleep(distance, so_speed);
        /* new location */
        ship->coord = *dest;
}

/**
 *  function to decide the next port based on availability and distance ???
 */
 

void convert_and_sleep(double param, int speed)
{
	double time_required;
	struct timespec sleep_time;
	
	/* calculate time required to terminate the operation
         * (in days)
         */
	time_required = param / speed;
	time_required *= SECONDS_IN_DAY; /* conversion in seconds */
	
	/* conversion in timespec to invoke nanosleep() */
	sleep_time.tv_sec = (int) time_required;
	sleep_time.tv_nsec = (long)(time_required - (double)sleep_time.tv_sec) * NANOSECONDS_IN_SECOND;
	
	nanosleep(&sleep_time, NULL);
}

struct data_cargo * generate_cargo()
{
	struct data_cargo *cargo = malloc(so_capacity * sizeof(*cargo));
	
	
}