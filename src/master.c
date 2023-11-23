#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "include/const.h"
#include "include/shm_general.h"
#include "include/shm_port.h"
#include "include/shm_ship.h"
#include "include/sem.h"

struct state {
	shm_general_t *general;
	shm_port_t *ports;
	shm_ship_t *ships;
	pid_t weather;
};

void signal_handler(int signal);
void signal_handler_init(void);

void run_ports(struct state *s);
void run_ships(struct state *s);
void run_weather(struct state *s);

pid_t run_process(char *name, int index);

void close_all(void);

struct state state;

int main(int argc, char *argv[])
{
	int id_sem_start;

	signal_handler_init();

	state.general = read_from_path("../constants.txt", &state.general);
	if (state.general == NULL) {
		exit(1);
	}

	state.ports = port_initialize(state.general);
	if (state.ports == NULL) {
		exit(1);
	}

	state.ships = ship_initialize(state.general);
	if (state.ships == NULL) {
		exit(1);
	}

	id_sem_start = sem_create(SEM_START_KEY, 1);
	sem_setval(id_sem_start, 0, 1);

	run_ports(&state);
	run_ships(&state);
	run_weather(&state);

	sem_execute_semop(get_sem_port_init_id(), 0, 0, 0);
	sem_execute_semop(id_sem_start, 0, -1, 0);


	alarm(1);

	while (1) {
		pause();
	}
}

void signal_handler_init(void)
{
	static struct sigaction sa;
	sigset_t mask;

	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;

	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGALRM, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
}

void run_ports(struct state *s)
{
	int i, sem_id;
	int n_port;
	pid_t pid;

	n_port = get_porti(s->general);

	sem_id = sem_create(SEM_PORTS_INITIALIZED_KEY, n_port);
	sem_setval(sem_id, 0, n_port);

	for (i = 0; i < n_port; i++) {
		pid = run_process("./port", i);
		port_shm_set_pid(state.ports, i, pid);
	}
}

void run_ships(struct state *s)
{
	int i;
	int n_ship = get_navi(s->general);
	pid_t pid;

	for (i = 0; i < n_ship; i++) {
		pid = run_process("./ship", i);
		ship_shm_set_pid(state.ships, i, pid);
	}
}

void run_weather(struct state *s)
{
	pid_t pid;

	pid = run_process("./weather", 1);

	state.weather = pid;
}

pid_t run_process(char *name, int index)
{
	pid_t process_pid;
	char *args[3], buf[10];
	if ((process_pid = fork()) == -1) {
		dprintf(2, "master.c: Error in fork.\n");
		close_all();
	} else if (process_pid == 0) {
		sprintf(buf, "%d", index);
		args[0] = name;
		args[1] = buf;
		args[2] = NULL;
		if (execve(name, args, NULL) == -1) {
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}

	return process_pid;
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGSEGV:
		dprintf(2, "master.c: Segmentation fault. Closing all.\n");
	case SIGTERM:
	case SIGINT:
		close_all();
	case SIGALRM:
		if (get_current_day(state.general) + 1 == get_days(state.general) + 1) {
			dprintf(1,
				"Reached last day of simulation. Terminating...\n");
			close_all();
		}
		increase_day(state.general);
		port_shm_send_signal_to_all_ports(
			state.ports, state.general, SIGDAY);
		ship_shm_send_signal_to_all_ships(
			state.ships, state.general, SIGDAY);
		kill(state.weather, SIGDAY);
		alarm(1);
		break;
	default:
		break;
	}
}

void close_all(void)
{
	ship_shm_send_signal_to_all_ships(state.ships, state.general, SIGINT);
	port_shm_send_signal_to_all_ports(state.ports, state.general, SIGINT);
	kill(state.weather, SIGINT);
	port_shm_delete(state.general);
	ship_shm_delete(state.general);

	sem_delete(get_sem_start_id());
	sem_delete(get_sem_port_init_id());
	sem_delete(get_sem_port_id());

	general_shm_delete(get_general_shm_id(state.general));

	exit(EXIT_SUCCESS);

}
