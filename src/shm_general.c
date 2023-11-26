#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/shm.h"

#include "include/const.h"
#include "include/shm_general.h"

struct shm_general {
	double so_lato;
	int so_days;
	int so_navi, so_speed, so_capacity;
	int so_porti, so_banchine, so_fill, so_loadspeed;
	int so_merci, so_size, so_min_vita, so_max_vita;
	int so_storm_duration, so_swell_duration, so_maelstrom;

	int current_day;

	int general_shm_id, ship_shm_id, port_shm_id, offer_shm_id, demand_shm_id;
	int msg_in_id, msg_out_id;
};

static void set_general_shm_id(shm_general_t *g);
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

	general_shm_attach(&data);

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
		if(value <= 0) {
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

	set_general_shm_id(data);
	return data;
}

/* General shared memory */
void general_shm_attach(shm_general_t **g)
{
	int shm_id;

	shm_id = shm_create(SHM_DATA_GENERAL_KEY, sizeof(shm_general_t));
	if (shm_id == -1) {
		dprintf(1, "do something\n");
	}
	*g = shm_attach(shm_id);
}

void general_shm_detach(shm_general_t *c)
{
	shm_detach(c);
}

void general_shm_delete(int id)
{
	shm_delete(id);
}

/* Setters */
void set_port_shm_id(shm_general_t *c, int id){c->port_shm_id = id;}
static void set_general_shm_id(shm_general_t *g){g->general_shm_id = shm_create(SHM_DATA_GENERAL_KEY, 0);}
void set_ship_shm_id(shm_general_t *c, int id){c->ship_shm_id = id;}
void set_offer_shm_id(shm_general_t *c, int id){c->offer_shm_id = id;}
void set_demand_shm_id(shm_general_t *c, int id){c->demand_shm_id = id;}
void set_msg_in_id(shm_general_t *c, int id){c->msg_in_id = id;}
void set_msg_out_id(shm_general_t *c, int id){c->msg_out_id = id;}

/* Getters */
int get_port_shm_id(shm_general_t *c)
{
	if (c == NULL) {
		return -1;
	}
	return c->port_shm_id;
}

int get_ship_shm_id(shm_general_t *c)
{
	if (c == NULL) {
		return -1;
	}
	return c->ship_shm_id;
}

int get_general_shm_id(shm_general_t *c){ return c->general_shm_id; }

int get_offer_shm_id(shm_general_t *c)
{
	if (c == NULL) {
		return -1;
	}
	return c->offer_shm_id;
}

int get_demand_shm_id(shm_general_t *c)
{
	if (c == NULL) {
		return -1;
	}
	return c->demand_shm_id;
}

int get_msg_in_id(shm_general_t *c){return c->msg_in_id;}
int get_msg_out_id(shm_general_t *c){return c->msg_out_id;}


/* Getters for simulation costants */
double get_lato(shm_general_t *c){ return c->so_lato; }
int get_days(shm_general_t *c){	return c->so_days; }
int get_navi(shm_general_t *c){	return c->so_navi; }
int get_speed(shm_general_t *c){ return c->so_speed; }
int get_capacity(shm_general_t *c){ return c->so_capacity; }
int get_porti(shm_general_t *c){ return c->so_porti; }
int get_banchine(shm_general_t *c){ return c->so_banchine; }
int get_fill(shm_general_t *c){	return c->so_fill; }
int get_load_speed(shm_general_t *c){ return c->so_loadspeed; }
int get_merci(shm_general_t *c){ return c->so_merci; }
int get_size(shm_general_t *c){	return c->so_size; }
int get_min_vita(shm_general_t *c){ return c->so_min_vita; }
int get_max_vita(shm_general_t *c){ return c->so_max_vita; }
int get_storm_duration(shm_general_t *c){ return c->so_storm_duration; }
int get_swell_duration(shm_general_t *c){ return c->so_swell_duration; }
int get_maelstrom(shm_general_t *c){ return c->so_maelstrom; }

int get_current_day(shm_general_t *c){ return c->current_day; }

void increase_day(shm_general_t *c){ c->current_day++; }







