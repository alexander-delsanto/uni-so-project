#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include "../lib/semaphore.h"

#include "include/const.h"
#include "include/shm_general.h"
#include "include/shm_port.h"
#include "include/shm_ship.h"
#include "include/shm_cargo.h"
#include "include/shm_offer_demand.h"
#include "include/msg_commerce.h"

struct state {
	shm_general_t *general;
	shm_port_t *ports;
	shm_ship_t *ships;
	shm_cargo_t *cargo;
	pid_t weather;
};

void signal_handler(int signal);
void signal_handler_init(void);

void run_ports(void);
void run_ships(void);
void run_weather(void);

pid_t run_process(char *name, int index);

void print_daily_report(void);
void print_final_report(void);
bool_t check_ships_all_dead(void);

void close_all(void);

struct state state;

int main(int argc, char *argv[])
{
	signal_handler_init();

	srand(time(NULL) * getpid());
	state.general = read_from_path("../constants.txt", &state.general);
	if (state.general == NULL) {
		exit(1);
	}
	shm_general_ipc_init(state.general);

	state.ports = shm_port_initialize(state.general);
	if (state.ports == NULL) {
		exit(1);
	}
	shm_port_ipc_init(state.general, state.ports);

	state.ships = shm_ship_initialize(state.general);
	if (state.ships == NULL) {
		exit(1);
	}

	state.cargo = shm_cargo_initialize(state.general);
	if (state.cargo == NULL) {
		exit(1);
	}


	run_ports();
	run_ships();
	run_weather();

	sem_execute_semop(sem_port_init_get_id(state.general), 0, 0, 0);
	sem_execute_semop(sem_start_get_id(state.general), 0, -1, 0);


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

void run_ports(void)
{
	int i, sem_id;
	int n_port;
	pid_t pid;

	n_port = get_porti(state.general);
	for (i = 0; i < n_port; i++) {
		pid = run_process("./port", i);
		shm_port_set_pid(state.ports, i, pid);
	}
}

void run_ships(void)
{
	int i;
	int n_ship = get_navi(state.general);
	pid_t pid;

	for (i = 0; i < n_ship; i++) {
		pid = run_process("./ship", i);
		shm_ship_set_pid(state.ships, i, pid);
	}
}

void run_weather(void)
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

void print_daily_report(void) {
	int i, qty, type;

	dprintf(1, "\nDaily report #%d:\n", get_current_day(state.general));
	dprintf(1, "**********CARGO**********\n");
	for(type = 0; type < get_merci(state.general); type++){
		dprintf(1, "Type %d:\n", type);
		dprintf(1, "\t%d available in ports;\n",
			shm_cargo_get_dump_available_in_port(state.cargo, type));
		dprintf(1, "\t%d available on ships;\n",
			shm_cargo_get_dump_available_on_ship(state.cargo, type));
		dprintf(1, "\t%d delivered to ports;\n",
			shm_cargo_get_dump_received_in_port(state.cargo, type));
		dprintf(1, "\t%d expired in ports;\n",
			shm_cargo_get_dump_expired_in_port(state.cargo, type));
		dprintf(1, "\t%d expired on ships;\n",
			shm_cargo_get_dump_expired_on_ship(state.cargo, type));
	}

	dprintf(1, "\n**********SHIPS**********\n");
	dprintf(1, "Number of ships at sea with cargo: %d\n",
		shm_ship_get_dump_with_cargo(state.general, state.ships));
	dprintf(1, "Number of ships at sea without cargo: %d\n",
		shm_ship_get_dump_without_cargo(state.general, state.ships));
	dprintf(1, "Number of ships at docks: %d\n",
		shm_ship_get_dump_at_dock(state.general, state.ships));

	dprintf(1, "\n**********PORTS**********\n");
	for (i = 0; i < get_porti(state.general); i++) {
		dprintf(1, "Port %d:\n", i);
		dprintf(1, "\t%d goods available;\n",
			shm_port_get_dump_cargo_available(state.ports, i));
		dprintf(1, "\t%d goods shipped until now;\n",
			shm_port_get_dump_cargo_shipped(state.ports, i));
		dprintf(1, "\t%d goods received until now;\n",
			shm_port_get_dump_cargo_received(state.ports, i));
		dprintf(1, "\tDocks used: %d/%d\n",
			shm_port_get_dump_used_docks(state.ports, i), shm_port_get_docks(state.ports, i));
	}

	dprintf(1, "\n**********WEATHER**********\n");
	dprintf(1, "%d ships slowed by the storm until now.\n",
		shm_ship_get_dump_had_storm(state.general, state.ships));

	dprintf(1, "List of ports affecting by the swell at the moment: \n");
	for (i = 0; i < get_porti(state.general); i++) {
		if(shm_port_get_dump_having_swell(state.ports, i) == TRUE)
			dprintf(1, "\tPort %d\n", i);
	}

	dprintf(1, "%d ships dead due to maelstrom today.\n",
		shm_ship_get_dump_had_maelstrom(state.general, state.ships));
}

void print_final_report(void) {
	int i, type, cnt;
	int n_port = get_porti(state.general);
	int n_ship = get_navi(state.general);
	int n_cargo = get_merci(state.general);

	dprintf(1, "\n\nFINAL REPORT:\n");
	dprintf(1, "**********SHIPS**********\n");
	dprintf(1, "Number of ships at sea with cargo: %d\n",
		shm_ship_get_dump_with_cargo(state.general, state.ships));
	dprintf(1, "Number of ships at sea without cargo: %d\n",
		shm_ship_get_dump_without_cargo(state.general, state.ships));
	dprintf(1, "Number of ships at at docks: %d\n",
		shm_ship_get_dump_at_dock(state.general, state.ships));
	dprintf(1, "**********CARGO**********\n");
	for(type = 0; type < n_cargo; type++){
		dprintf(1, "Type %d:\n", type);
		dprintf(1, "\t%d generated since the beginning;\n",
			shm_cargo_get_dump_total_generated(state.cargo, type));
		dprintf(1, "\t%d unsold in port;\n",
			shm_cargo_get_dump_available_in_port(state.cargo, type));
		dprintf(1, "\t%d expired in ports;\n",
			shm_cargo_get_dump_expired_in_port(state.cargo, type));
		dprintf(1, "\t%d expired on ships;\n",
			shm_cargo_get_dump_expired_on_ship(state.cargo, type));
		dprintf(1, "\t%d delivered to ports;\n",
			shm_cargo_get_dump_received_in_port(state.cargo, type));
	}

	dprintf(1, "**********PORTS**********\n");
	for (i = 0; i < n_port; i++) {
		dprintf(1, "Port %d:\n", i);
		dprintf(1, "\t%d goods available;\n",
			shm_port_get_dump_cargo_available(state.ports, i));
		dprintf(1, "\t%d goods shipped;\n",
			shm_port_get_dump_cargo_shipped(state.ports, i));
		dprintf(1, "\t%d goods received;\n",
			shm_port_get_dump_cargo_received(state.ports, i));
	}

	/* TODO: Indicate the port that has offered the most goods and the one that has requested the most goods. */

	dprintf(1, "**********WEATHER**********\n");
	dprintf(1, "%d ships slowed by the storm.\n",
		shm_ship_get_dump_had_storm(state.general, state.ships));
	dprintf(1, "List of ports affected by the swell: \n");
	for (i = 0; i < n_port; i++) {
		if(shm_port_get_dump_swell_final(state.ports, i) == TRUE)
			dprintf(1, "\tPort %d\n", i);
	}
	dprintf(1, "%d ships dead due to maelstrom.\n",
		shm_ship_get_dump_is_dead(state.ships, n_ship));
}

bool_t check_ships_all_dead(void)
{
	int i;
	bool_t res = TRUE;
	for (i = 0; i < get_navi(state.general); i++) {
		if (!shm_ship_get_is_dead(state.ships, i)) {
			res = FALSE;
			break;
		}
	}
	return res;
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
		print_daily_report();
		if (check_ships_all_dead()) {
			dprintf(1, "All ships are dead. Terminating...\n");
			close_all();
		}
		if (get_current_day(state.general) + 1 == get_days(state.general) + 1) {
			dprintf(1,
				"Reached last day of simulation. Terminating...\n");
			close_all();
		}

		increase_day(state.general);
		kill(state.weather, SIGDAY);
		alarm(1);
		break;
	default:
		break;
	}
}

void close_all(void)
{
	print_final_report();

	kill(state.weather, SIGINT);
	shm_ship_send_signal_to_all_ships(state.ships, state.general, SIGINT);
	shm_port_send_signal_to_all_ports(state.ports, state.general, SIGINT);
	while (wait(NULL) > 0);

	msgctl(msg_in_get_id(state.general), IPC_RMID, NULL);
	msgctl(msg_out_get_id(state.general), IPC_RMID, NULL);

	sem_delete(sem_start_get_id(state.general));
	sem_delete(sem_port_init_get_id(state.general));
	sem_delete(shm_port_get_sem_docks_id(state.ports));
	sem_delete(sem_cargo_get_id(state.general));

	shm_port_delete(state.general);
	shm_ship_delete(state.general);
	shm_offer_demand_delete(state.general);
	shm_cargo_delete(state.general);

	shm_general_delete(shm_general_get_id(state.general));

	exit(EXIT_SUCCESS);

}
