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

struct state {
	shm_general_t *general;
	shm_port_t *ports;
	shm_ship_t *ships;
	pid_t weather;

	bool_t running;
};

void signal_handler(int signal);
struct sigaction *signal_handler_init(void);

void run_ports(struct state *s);
void run_ships(struct state *s);
void run_weather(struct state *s);

pid_t run_process(char *name, int index);

void close_all(void);

int day = 0;
struct state state;

int main(int argc, char *argv[])
{
	pid_t pid;
	struct sigaction *sa;

	pid = getpid();

	state.running = TRUE;
	dprintf(1, "ciao1\n");

	state.general = read_from_path("../constants.txt");
	dprintf(1, "ciao2\n");
	if (state.general == NULL) {
		exit(1);
	}

	state.ports = port_initialize(state.general);
	if (state.ports == NULL) {
		exit(1);
	}
	dprintf(1, "ciao3\n");

	state.ships = ship_initialize(state.general);
	if (state.ships == NULL) {
		exit(1);
	}
	dprintf(1, "ciao4\n");


	sa = signal_handler_init();
	/*run_ports(state.general, &state);
	run_ships(general, &state);
	run_weather(general, &state);*/

	alarm(1);

	while (state.running == TRUE) {
		pause();
	}

	close_all();

	return EXIT_SUCCESS;
}

struct sigaction *signal_handler_init(void)
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

	return &sa;
}

void run_ports(struct state *s)
{
	int i;
	int n_port;
	pid_t pid;

	n_port = get_porti(s->general);
	for (i = 0; i < n_port; i++) {
		run_process("./port", i);
		port_shm_set_pid(state.ports, i, pid);
	}
}

void run_ships(struct state *s)
{
	int i;
	int n_ship;
	pid_t pid;

	for (i = 0; i < n_ship; i++) {
		run_process("./ship", i);
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
	} else if (process_pid == 0) {
		sprintf(buf, "%d", index);
		args[0] = name;
		args[1] = buf;
		args[2] = NULL;
		if (execve(name, args, NULL) == -1) {
			perror("execve");
			exit(EXIT_SUCCESS);
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
	case SIGALRM:
		dprintf(1, "porco cane: %d\n", day);
		if (day >= 5) {
			port_shm_send_signal_to_all_ports(
				state.ports, state.general, SIGKILL);
			ship_shm_send_signal_to_all_ships(
				state.ships, state.general, SIGKILL);
			kill(state.weather, SIGKILL);
			state.running = FALSE;
		}
		day++;
		port_shm_send_signal_to_all_ports(state.ports, state.general,
						  SIGDAY);
		ship_shm_send_signal_to_all_ships(state.ships, state.general,
						  SIGDAY);

		alarm(1);
		break;
	default:
		break;
	}
}

void close_all(void)
{
	int conf_shm_id;

	port_shm_detach(state.ports);
	port_shm_delete(state.general);
	ship_shm_detach(state.ships);
	ship_shm_delete(state.general);
	conf_shm_id = get_general_shm_id(state.general);
	general_shm_detach(state.general);
	general_shm_delete(conf_shm_id);
}
