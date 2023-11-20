#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../lib/semaphore.h"

#include "header/cargo.h"
#include "header/utils.h"
#include "header/ipc_utils.h"
#include "header/types.h"

typedef struct settings {
	int id;
	int sem_id;
	pid_t master;
	pid_t pid;
	coord_t coordinates;
	cargo_t *cargo;
} settings_t;

void generate_coordinates(settings_t *settings);
void put_coordinates_shm(settings_t *settings);
void generate_docks(settings_t *settings);
void generate_cargo(settings_t *settings);

void signal_handler(int signal);

void send_report(void);

void close_all(settings_t settings);

int main(int argc, char *argv[])
{
	settings_t settings;

	struct sigaction sa;
	sigset_t mask;

	bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_handler;
	/* Signal handler initialization */
	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGALRM, &sa, NULL);

	attach_process_to_shm();

	settings.id = atoi(argv[2]);
	settings.pid = getpid();
	settings.master = getppid();
	generate_coordinates(&settings);
	generate_docks(&settings);
	generate_cargo(&settings);

	put_coordinates_shm(&settings);

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
	settings->cargo = cargo_generate(SO_MERCI, SO_FILL);
	cargo_generate_goodies(settings->cargo, SO_SIZE, SO_MIN_VITA,
			       SO_MAX_VITA);
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGSEGV:
		dprintf(2, "master.c: Segmentation fault. Closing all.\n");
		break;
	case SIGDAY:
		send_report();
		break;
	}
}

void close_all(settings_t settings)
{
	/* TODO */
	sem_delete(settings.sem_id);
}
