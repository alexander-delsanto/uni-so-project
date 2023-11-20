#ifndef _SHARED_MEMORY_H
#define _SHARED_MEMORY_H

#include "utils.h"
#include <sys/types.h>
#include <sys/shm.h>

#define SHM_DATA_GENERAL_KEY 0x1fff
#define SHM_DATA_PORTS_KEY 0x2fff
#define SHM_DATA_SHIPS_KEY 0x3fff
#define SHM_DATA_CARGO_KEY 0x4fff

#define SEM_START_KEY 0x10fff

struct data_general {
	double so_lato;
	int so_days;
	int so_navi, so_speed, so_capacity;
	int so_porti, so_banchine, so_fill, so_loadspeed;
	int so_merci, so_size, so_min_vita, so_max_vita;
	int so_storm_duration, so_swell_duration, so_maelstrom;

	int current_day;
};

struct coordinates {
	double x;
	double y;
};

struct data_port {
	pid_t pid;
	struct coordinates coord;
	int num_docks;

	int dump_cargo_available;
	int dump_cargo_shipped;
	int dump_cargo_received;
	int dump_used_docks;
	int dump_ships_arrived;
	bool_t dump_had_swell;
};

struct data_ship {
	pid_t pid;
	struct coordinates coord;

	bool_t is_moving;
	bool_t is_dead;

	bool_t dump_had_storm;
	bool_t dump_had_maelstrom;
};

struct data_cargo {
	int weight; /* in [1, SO_SIZE] */
	int time_of_life; /* in [SO_MIN_VITA, SO_MAX_VITA] */
};

#endif
