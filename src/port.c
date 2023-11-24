#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "include/const.h"
#include "include/sem.h"
#include "include/shm_general.h"
#include "include/types.h"
#include "include/utils.h"
#include "include/shm_port.h"

struct state {
	int id;
	int sem_id;
	pid_t pid;
	shm_general_t *general;
	shm_port_t *port;

	int current_day;
};

void signal_handler(int signal);
void signal_handler_init(void);

void generate_coordinates(void);
void generate_docks(void);

void close_all(void);

struct state state;

int main(int argc, char *argv[])
{
	int day;
	bzero(&state, sizeof(struct state));
	signal_handler_init();

	state.id = (int)strtol(argv[1], NULL, 10);
	state.pid = getpid();

	general_shm_attach(&state.general);
	if (state.general == NULL) {
		close_all();
		exit(1);
	}
	state.port = port_shm_attach(state.general);
	srand(time(NULL) * getpid());
	generate_coordinates();
/*	generate_docks();*/

	sem_execute_semop(get_sem_port_init_id(), 0, -1, 0);
	sem_execute_semop(get_sem_start_id(), 0, 0, 0);

	while (1) {
		day = get_current_day(state.general);
		if (state.current_day < day) {
			/* TODO: new day operations */
			dprintf(1, "port %d: day %d to day %d.\n", state.id, state.current_day, day);
			state.current_day = day;

		}
	}
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
		coordinates.y = RANDOM_DOUBLE(0, max);
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

void signal_handler_init(void)
{
	static struct sigaction sa;
	sigset_t mask;

	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;

	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGSWELL, &sa, NULL);

}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
		break;
	case SIGSWELL:
		dprintf(1,"Port %d: Received SIGSWELL signal. Sleeping for %f seconds...\n",
			state.id, get_swell_duration(state.general) / 24.0);
		convert_and_sleep(get_swell_duration(state.general) / 24.0);
		dprintf(1, "port %d woke up.\n", state.id);
		break;
	case SIGSEGV:
		dprintf(1, "Received SIGSEGV signal.\n");
		dprintf(2, "ship.c: id: %d: Segmentation fault. Terminating.\n",
			state.id);
	case SIGINT:
		dprintf(1, "port %d: received SIGINT\n", state.id);
		close_all();
	default:
		break;
	}
}

void close_all(void)
{
	port_shm_detach(state.port);
	general_shm_detach(state.general);
	exit(EXIT_SUCCESS);
}
