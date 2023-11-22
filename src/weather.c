#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "include/utils.h"
#include "include/sem.h"
#include "include/const.h"
#include "include/shm_general.h"
#include "include/shm_port.h"
#include "include/shm_ship.h"

void signal_handler(int signal);
void signal_handler_init(void);

void send_storm_signal(void);
void send_swell_signal(void);
void start_timer(double timer_interval);

void send_maelstrom_signal(void);

void close_all(void);

struct state {
	shm_general_t *general;
	shm_port_t *ports;
	shm_ship_t *ships;
};

struct state state;

int main(int argc, char *argv[])
{
	signal_handler_init();

	general_shm_attach(&state.general);
	state.ports = port_shm_attach(state.general);
	state.ships = ship_shm_attach(state.general);

	srand(getpid() * time(NULL));

	sem_execute_semop(get_sem_start_id(), 0, 0, 0);

	start_timer(get_maelstrom(state.general) / 24.0);

	while (1) {
		pause();
	}
}

void signal_handler_init(void)
{
	struct sigaction sa;
	bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_handler;

	/* Signal handler initialization */
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGALRM, &sa, NULL);
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
		send_storm_signal();
		send_swell_signal();
		break;
	case SIGALRM:
		send_maelstrom_signal();
		break;
	case SIGSEGV:
		dprintf(2, "weather.c: Segmentation fault. Closing.\n");
	case SIGINT:
		close_all();
		break;
	default:
		break;
	}
}

void send_storm_signal(void)
{
	int i, n_ships, target_ship;
	n_ships = get_navi(state.general);
	target_ship = RANDOM_INTEGER(0, n_ships - 1);

	for (i = 0; i < n_ships; i++) {
		if (!ship_shm_get_dead(state.ships, target_ship) &&
		    ship_shm_get_is_moving(state.ships, target_ship)) {
			ship_shm_send_signal_to_ship(state.ships, target_ship, SIGSTORM);
			return;
		}
		target_ship = (target_ship + 1) % n_ships;
	}
}

void send_maelstrom_signal(void)
{
	int i, n_ships, target_ship;
	n_ships = get_navi(state.general);
	target_ship = RANDOM_INTEGER(0, n_ships - 1);

	for (i = 0; i < n_ships; i++) {
		if (!ship_shm_get_dead(state.ships, target_ship)) {
			ship_shm_send_signal_to_ship(state.ships, target_ship, SIGMAELSTROM);
			return;
		}
		target_ship = (target_ship + 1) % n_ships;
	}
}

void send_swell_signal(void)
{
	int target_port = RANDOM_INTEGER(0, (get_porti(state.general) - 1));
	port_shm_send_signal_to_port(state.ports, target_port, SIGSWELL);
}

void start_timer(double timer_interval)
{
	struct itimerval timer;
	int sec, u_sec;

	sec = (int)timer_interval;
	u_sec = (timer_interval - sec) * 1e6;

	timer.it_value.tv_sec = sec;
	timer.it_value.tv_usec = u_sec;
	timer.it_interval.tv_sec = sec;
	timer.it_interval.tv_usec = u_sec;

	setitimer(ITIMER_REAL, &timer, NULL);
}

void close_all(void)
{
	port_shm_detach(state.ports);
	ship_shm_detach(state.ships);
	general_shm_detach(state.general);

	exit(EXIT_SUCCESS);
}
