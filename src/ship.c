#define _GNU_SOURCE
#include "header/shared_memory.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define NANOSECONDS_IN_SECOND 1000000000L /* constant for conversion to ns */
#define SECONDS_IN_DAY 86400 /* seconds in a day */

int base = 100; // base of the map
int capacity = 1000; // capacity of the ship
int speed = 10; // speed of the ship
struct data_ship ship;
struct data_port destination;	/* we need to be able to access ports data */

/*
 *  for the stack we need a struct of cargo struct ???
 */

void init_location(struct data_ship *ship);
void signal_handler(int signal);
void move(struct data_ship *ship);


int main(int argc, char const *argv[])
{
        /* obtain the necessary data from shm */

        /* init signals */
        
        /* generate a random location on the map */
        init_location(&ship);




        return 0;
}

void init_location()
{
        /* generate a random location on the map */
        srand(time(NULL) * getpid());
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
        double time_required;
        struct timespec sleep_time;

        /* calculate distance between actual position and destination */
        distance = sqrt(destination.coord.x - ship->coord.x, 2) + pow(destination->coord.y - ship->coord.y, 2));

        /* calculate time required to cover the distance at ship's speed (in km/day) */
        time_required = distance / speed;
        time_required *= SECONDS_IN_DAY; /* conversion in seconds */

        /* conversion in timespec to invoke nanosleep */
        sleep_time.tv_sec = (int) time_required;
        sleep_time.tv_nsec = (time_required - sleep_time.tv_sec) * NANOSECONDS_IN_SECOND;

        nanosleep(&sleep_time, NULL);

        /* new location */
        ship->coord = destination.coord;
}

/**
 *  function to decide the next port based on availability and distance ???
 */
