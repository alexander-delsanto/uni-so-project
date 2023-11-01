#ifndef _SHARED_MEMORY_H
#define _SHARED_MEMORY_H

#include "utils.h"
#include <sys/types.h>
#include <sys/shm.h>

#define SHM_DATA_GENERAL_KEY 0x1fff
#define SHM_DATA_PORTS_KEY 0x2fff

typedef struct _data_general{
	double so_lato;
	int so_days;
	int so_navi, so_speed, so_capacity;
	int so_porti, so_banchine, so_fill, so_loadspeed;
	int so_merci, so_size, so_min_vita, so_max_vita;
	int so_storm_duration, so_swell_duration, so_maelstrom;
} data_general;

struct data_port{
	pid_t pid;
	double x_coord;
	double y_coord;
};




#endif
