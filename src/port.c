#define _GNU_SOURCE

#include <stdlib.h>
#include <unistd.h>

#include "../lib/semaphore.h"

#include "header/utils.h"
#include "header/ipc_utils.h"
#include "header/types.h"

typedef struct settings {
	int id;
	int sem_id;
	coord_t coordinates;
} settings_t;

void generate_coordinates(settings_t *settings);
void put_coordinates_shm(settings_t *settings);
void generate_docks(settings_t *settings);
void generate_cargo(settings_t *settings);

void send_report(void);

void close_all(settings_t settings);

int main(int argc, char *argv[])
{
	settings_t settings;

	attach_process_to_shm();

	settings.id = atoi(argv[2]);
	generate_coordinates(&settings);
	generate_docks(&settings);
	generate_cargo(&settings);

	put_coordinates_shm(&settings);

	// Funzione carico/scarico
	// Genera report

	close_all(settings);

	return EXIT_SUCCESS;
}

void generate_coordinates(settings_t *settings)
{
	coord_t coordinates;
	double max;

	max = SO_LATO;

	switch (settings->id) {
	case 0:
		coordinates.x = 0;
		coordinates.y = 0;
		break;
	case 1:
		coordinates.x = 0;
		coordinates.y = max;
		break;
	case 2:
		coordinates.x = max;
		coordinates.y = 0;
		break;
	case 3:
		coordinates.x = max;
		coordinates.y = max;
		break;
	default:
		coordinates.x = RANDOM_DOUBLE(0, max);
		coordinates.x = RANDOM_DOUBLE(0, max);
		break;
	}

	settings->coordinates = coordinates;
}

void generate_docks(settings_t *settings)
{
	settings->sem_id = sem_create(100, RANDOM_INTEGER(1, SO_BANCHINE));
}

void generate_cargo(settings_t *settings)
{
	/* TODO */
}

void close_all(settings_t settings)
{
	/* TODO */
	sem_delete(settings.sem_id);
}
