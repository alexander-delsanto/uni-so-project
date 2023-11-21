#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/shm.h"

#include "include/const.h"
#include "include/shm_general.h"

#define NUM_CONST 16

struct shm_general {
	double so_lato;
	int so_days;
	int so_navi, so_speed, so_capacity;
	int so_porti, so_banchine, so_fill, so_loadspeed;
	int so_merci, so_size, so_min_vita, so_max_vita;
	int so_storm_duration, so_swell_duration, so_maelstrom;

	int current_day;

	int general_shm_id, pid_shm_id, ship_shm_id, port_shm_id;
};

shm_general_t *copy_to_shm(shm_general_t *c);

shm_general_t *read_from_path(char *path)
{
	FILE *file;
	char c;
	char buffer[100];
	int n_char, counter = 0;
	double value;
	shm_general_t *data;

	data = (shm_general_t *)malloc(sizeof(shm_general_t));
	if (data == NULL) {
		return NULL;
	}

	file = fopen(path, "r");
	if (file == NULL) {
		return NULL;
	}

	/* TODO: usare fgets e sscanf */
	while ((n_char = fscanf(file, "%lf", &value)) != EOF) {
		if (n_char != 0) {
			if (counter >= NUM_CONST) {
				fclose(file);
				return NULL;
			}
			if (value <= 0) {
				fclose(file);
				return NULL;
			}
			if (counter <= 0) {
				data->so_lato = value;
			} else {
				(&data->so_days)[counter - 1] = (int)value;
			}
			counter++;
		}

		fscanf(file, "%*[ \t]");
		/* TODO: rimuovere fgetc */
		if ((c = fgetc(file)) == '#') {
			fscanf(file, "%*[^\n]");
		} else if (!(c >= '0' && c <= '9')) {
			fclose(file);
			return NULL;
		}
	}

	data->current_day = 0;

	fclose(file);

	return copy_to_shm(data);
}

shm_general_t *copy_to_shm(shm_general_t *c)
{
	int shm_id;
	shm_general_t *data;

	shm_id = shm_create(SHM_DATA_GENERAL_KEY, sizeof(shm_general_t));
	if (shm_id == -1) {
		return NULL;
	}

	c->general_shm_id = shm_id;
	data = shm_attach(shm_id);
	memcpy(data, c, sizeof(shm_general_t));
}

shm_general_t *general_shm_attach(void)
{
	int shm_id;
	shm_general_t *data;

	shm_id = shm_create(SHM_DATA_GENERAL_KEY, sizeof(shm_general_t));
	if (shm_id == -1) {
		return NULL;
	}
	data = shm_attach(shm_id);

	return data;
}

void set_port_shm_id(shm_general_t *c, int id)
{
	c->port_shm_id = id;
}

void set_ship_shm_id(shm_general_t *c, int id)
{
	c->ship_shm_id = id;
}

void set_pid_shm_id(shm_general_t *c, int id)
{
	c->pid_shm_id = id;
}

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

int get_pid_shm_id(shm_general_t *c)
{
	if (c == NULL) {
		return -1;
	}
	return c->pid_shm_id;
}

double get_lato(shm_general_t *c)
{
	return c->so_lato;
}

int get_days(shm_general_t *c)
{
	return c->so_days;
}

int get_navi(shm_general_t *c)
{
	return c->so_navi;
}

int get_speed(shm_general_t *c)
{
	return c->so_speed;
}

int get_capacity(shm_general_t *c)
{
	return c->so_capacity;
}

int get_porti(shm_general_t *c)
{
	return c->so_porti;
}

int get_banchine(shm_general_t *c)
{
	return c->so_banchine;
}

int get_fill(shm_general_t *c)
{
	return c->so_fill;
}

int get_load_speed(shm_general_t *c)
{
	return c->so_loadspeed;
}

int get_merci(shm_general_t *c)
{
	return c->so_merci;
}

int get_size(shm_general_t *c)
{
	return c->so_size;
}

int get_min_vita(shm_general_t *c)
{
	return c->so_min_vita;
}

int get_max_vita(shm_general_t *c)
{
	return c->so_max_vita;
}

int get_storm_duration(shm_general_t *c)
{
	return c->so_storm_duration;
}

int get_swell_duration(shm_general_t *c)
{
	return c->so_swell_duration;
}

int get_maelstrom(shm_general_t *c)
{
	return c->so_maelstrom;
}

int get_current_day(shm_general_t *c)
{
	return c->current_day;
}

void increase_day(shm_general_t *c)
{
	c->current_day++;
}

int get_general_shm_id(shm_general_t *c)
{
	return c->general_shm_id;
}

void general_shm_detach(shm_general_t *c)
{
	shm_detach(c);
}
void general_shm_delete(int id)
{
	shm_delete(id);
}
