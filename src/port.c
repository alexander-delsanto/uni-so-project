#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>

#include "../lib/semaphore.h"

#include "include/const.h"
#include "include/shm_general.h"
#include "include/types.h"
#include "include/utils.h"
#include "include/shm_port.h"
#include "include/expired.h"

struct state {
	int id;
	int sem_id;
	pid_t master;
	pid_t pid;
	shm_general_t *general;
	shm_port_t *port;
	expired_t *exp;
	bool_t running;
};

void signal_handler(int signal);
struct sigaction *signal_handler_init(void);

void generate_coordinates(void);
void generate_docks(void);

void close_all(void);

struct state state;

int main(int argc, char *argv[])
{
	struct sigaction *sa;

	state.running = TRUE;
	state.id = (int)strtol(argv[1], NULL, 10);
	state.pid = getpid();
	state.master = getppid();

	state.general = general_shm_attach();
	if (state.general == NULL) {
		close_all();
		exit(1);
	}
	state.port = port_shm_attach(state.general);
	state.exp = expired_init(state.general);
	generate_coordinates();
	generate_docks();
	sa = signal_handler_init();

	while (state.running == TRUE) {
	}

	close_all();

	return EXIT_SUCCESS;
}

void generate_coordinates(void)
{
	struct coord coordinates;
	double max;

	max = get_lato(state.general);

	switch (state.id) {
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

	port_shm_set_coordinates(state.port, state.id, coordinates);
}

void generate_docks(void)
{
	int n;

	n = RANDOM_INTEGER(1, get_banchine(state.general));

	state.sem_id = sem_create(100, n);

	port_shm_set_docks(state.port, state.id, n);
}

struct sigaction *signal_handler_init(void)
{
	static struct sigaction sa;
	sigset_t mask;

	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;

	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

	return &sa;
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
		dprintf(1, "Port %d: Received SIGDAY signal. Current day: %d\n",
			state.id, get_current_day(state.general));
		expired_new_day(state.exp, state.general);
		port_shm_remove_expired(state.port, state.exp, state.general);
		port_shm_generate_cargo(state.port, state.id, state.general);
		break;
	case SIGSWELL:
		dprintf(1,
			"Port %d: Received SIGSWELL signal. Current day: %d\n",
			state.id, get_current_day(state.general));
		/* TODO */
		break;
	case SIGSEGV:
		dprintf(1, "Received SIGSEGV signal.\n");
		dprintf(2, "ship.c: id: %d: Segmentation fault. Terminating.\n",
			state.id);
		break;
	case SIGINT:
		state.running = FALSE;
	default:
		break;
	}
}

void close_all(void)
{
	sem_delete(state.sem_id);
	port_shm_detach(state.port);
	general_shm_detach(state.general);
}
