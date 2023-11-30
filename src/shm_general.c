#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/shm.h"

#include "include/const.h"
#include "include/shm_general.h"
#include "include/msg_commerce.h"
#include "../lib/semaphore.h"

struct shm_general {
	double so_lato;
	int so_days;
	int so_navi, so_speed, so_capacity;
	int so_porti, so_banchine, so_fill, so_loadspeed;
	int so_merci, so_size, so_min_vita, so_max_vita;
	int so_storm_duration, so_swell_duration, so_maelstrom;

	int current_day;

	int general_shm_id, ship_shm_id, port_shm_id, cargo_shm_id;
	int offer_shm_id, demand_shm_id;
	int msg_in_id, msg_out_id;
	int sem_start_id, sem_port_init_id, sem_dump_id;
};

/**
 * @brief Sets the shared memory ID for the general information structure.
 * @param g Pointer to the shm_general_t structure.
 */
static void shm_general_set_id(shm_general_t *g);
void remove_comment(char *str);

void remove_comment(char *str) {
	char *comment_start = strchr(str, '#');
	if (comment_start != NULL) {
		*comment_start = '\0';
	}
}

shm_general_t *read_from_path(char *path, shm_general_t **g)
{
	FILE *file;
	char buffer[100];
	int counter = 0;
	double value;
	shm_general_t *data = *g;

	shm_general_attach(&data);

	file = fopen(path, "r");
	if (file == NULL) {
		return NULL;
	}

	while(fgets(buffer, sizeof(buffer), file) != NULL) {
		remove_comment(buffer);
		if (buffer[0] == '\n' || buffer[0] == '\r' ||
		    buffer[0] == '\0') {
			continue;
		}

		if(counter >= NUM_CONST) {
			fclose(file);
			return NULL;
		}
		value = strtod(buffer, NULL);
		if(value < 0) {
			fclose(file);
			return NULL;
		}
		if(counter <= 0) {
			data->so_lato = value;
		} else {
			(&data->so_days)[counter - 1] = (int)value;
		}
		counter++;
	}

	data->current_day = 0;
	fclose(file);

	shm_general_set_id(data);
	return data;
}

void shm_general_ipc_init(shm_general_t *g)
{
	/* Semaphores */
	g->sem_start_id = sem_create(SEM_START_KEY, 1);
	sem_setval(g->sem_start_id, 0, 1);
	g->sem_dump_id = sem_create(SEM_DUMP_KEY, 1);
	sem_setval(g->sem_dump_id, 0, 1);
	g->sem_port_init_id = sem_create(SEM_PORTS_INITIALIZED_KEY, g->so_porti);
	sem_setval(g->sem_port_init_id, 0, g->so_porti);

	/* Message queues */
	g->msg_in_id = msg_commerce_in_port_init();
	g->msg_out_id = msg_commerce_out_port_init();
}

/* General shared memory */
void shm_general_attach(shm_general_t **g)
{
	int shm_id;

	shm_id = shm_create(SHM_DATA_GENERAL_KEY, sizeof(shm_general_t));
	if (shm_id == -1) {
		dprintf(1, "do something\n");
	}
	*g = shm_attach(shm_id);
}

void shm_general_detach(shm_general_t *g){shm_detach(g);}
void shm_general_delete(int id){shm_delete(id);}

/* Setters */
static void shm_general_set_id(shm_general_t *g){g->general_shm_id = shm_create(SHM_DATA_GENERAL_KEY, 0);}
void shm_ship_set_id(shm_general_t *g, int id){g->ship_shm_id = id;}
void shm_port_set_id(shm_general_t *g, int id){g->port_shm_id = id;}
void shm_cargo_set_id(shm_general_t *g, int id){g->cargo_shm_id = id;}
void shm_offer_set_id(shm_general_t *g, int id){g->offer_shm_id = id;}
void shm_demand_set_id(shm_general_t *g, int id){g->demand_shm_id = id;}

/* Getters */
int shm_general_get_id(shm_general_t *g){ return g->general_shm_id; }
int shm_ship_get_id(shm_general_t *g){return g->ship_shm_id;}
int shm_port_get_id(shm_general_t *g){return g->port_shm_id;}
int shm_cargo_get_id(shm_general_t *g){return g->cargo_shm_id;}
int shm_offer_get_id(shm_general_t *g){	return g->offer_shm_id;}
int shm_demand_get_id(shm_general_t *g){return g->demand_shm_id;}

int sem_start_get_id(shm_general_t *g){return g->sem_start_id;}
int sem_port_init_get_id(shm_general_t *g){return g->sem_port_init_id;}
int sem_dump_get_id(shm_general_t *g){return g->sem_dump_id;}

int msg_in_get_id(shm_general_t *g){return g->msg_in_id;}
int msg_out_get_id(shm_general_t *g){return g->msg_out_id;}


/* Getters for simulation costants */
double get_lato(shm_general_t *g){ return g->so_lato; }
int get_days(shm_general_t *g){	return g->so_days; }
int get_navi(shm_general_t *g){	return g->so_navi; }
int get_speed(shm_general_t *g){ return g->so_speed; }
int get_capacity(shm_general_t *g){ return g->so_capacity; }
int get_porti(shm_general_t *g){ return g->so_porti; }
int get_banchine(shm_general_t *g){ return g->so_banchine; }
int get_fill(shm_general_t *g){	return g->so_fill; }
int get_load_speed(shm_general_t *g){ return g->so_loadspeed; }
int get_merci(shm_general_t *g){ return g->so_merci; }
int get_size(shm_general_t *g){	return g->so_size; }
int get_min_vita(shm_general_t *g){ return g->so_min_vita; }
int get_max_vita(shm_general_t *g){ return g->so_max_vita; }
int get_storm_duration(shm_general_t *g){ return g->so_storm_duration; }
int get_swell_duration(shm_general_t *g){ return g->so_swell_duration; }
int get_maelstrom(shm_general_t *g){ return g->so_maelstrom; }

int get_current_day(shm_general_t *g){ return g->current_day; }

void increase_day(shm_general_t *g){ g->current_day++; }







