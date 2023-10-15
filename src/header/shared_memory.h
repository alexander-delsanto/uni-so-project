#ifndef _SHARED_MEMORY_H
#define _SHARED_MEMORY_H

#include "utils.h"
#include <sys/types.h>
#include <sys/shm.h>

#define SHM_DATA_KEY 0x1fff

id_shm_t get_shm(key_t shm_key, size_t size);
void *attach_shm(id_shm_t shm_id, int flags);
void detach_shm(void *shm_ptr);

struct data{
	double so_lato;
	int so_days;
	int so_navi, so_speed, so_capacity;
	int so_porti, so_banchine, so_fill, so_loadspeed;
	int so_merci, so_size, so_min_vita, so_max_vita;
	int so_storm_duration, so_swell_duration, so_maelstrom;
};


#endif
