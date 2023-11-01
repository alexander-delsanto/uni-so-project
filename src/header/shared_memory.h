#ifndef _SHARED_MEMORY_H
#define _SHARED_MEMORY_H

#include "utils.h"
#include <sys/types.h>
#include <sys/shm.h>

#define SHM_DATA_GENERAL_KEY 0x1fff
#define SHM_DATA_PORTS_KEY 0x2fff
#define SHM_DATA_SHIPS_KEY 0x3fff
#define SHM_DATA_CARGO_KEY 0x4fff


struct data_general{
	double so_lato;
	int so_days;
	int so_navi, so_speed, so_capacity;
	int so_porti, so_banchine, so_fill, so_loadspeed;
	int so_merci, so_size, so_min_vita, so_max_vita;
	int so_storm_duration, so_swell_duration, so_maelstrom;
};

struct coordinates{
	double x;
	double y;
};

struct data_port{
	pid_t pid;
	struct coordinates coord;
};

struct data_ship{
	pid_t pid;
	struct coordinates coord;
};
struct data_cargo{
	int weight;		/* in [1, SO_SIZE] */
	int time_of_life; 	/* in [SO_MIN_VITA, SO_MAX_VITA] */
};



#endif
