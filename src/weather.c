#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "include/const.h"
#include "include/shm_config.h"
#include "include/shm_port.h"
#include "include/shm_ship.h"

void signal_handler(int signal);
struct sigaction *signal_handler_init(void);

void send_storm_signal(void);
void send_swell_signal(void);
void start_timer(double timer_interval);

void send_maelstrom_signal(void);

void close_all(void);

struct state {
	shm_config_t *config;
	shm_port_t *ports;
	shm_ship_t *ships;
};

struct state state;

int main(int argc, char *argv[])
{
	struct sigaction *sa;

	sa = signal_handler_init();

	state.config = config_shm_attach();
	state.ports = port_shm_attach(state.config);
	state.ships = ship_shm_attach(state.config);

	srand(getpid() * time(NULL));
	start_timer(get_maelstrom(state.config) / 24.0);

	while (1) {
		pause();
	}
}

struct sigaction *signal_handler_init(void)
{
	static struct sigaction sa;
	sigset_t mask;
	bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_handler;

	/* Signal handler initialization */
	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);

	sigemptyset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGALRM, &sa, NULL);

	return &sa;
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
		close_all();
		break;
	case SIGINT:
		close_all();
		break;
	default:
		break;
	}
}

void send_storm_signal(void)
{
	int id;

	id = ship_shm_get_random_kill(state.ships, state.config);
	ship_shm_send_signal_to_ship(state.ships, id, SIGSTORM);
}

void send_maelstrom_signal(void)
{
	int id;

	id = ship_shm_get_random_maelstrom(state.ships, state.config);
	ship_shm_send_signal_to_ship(state.ships, id, SIGMAELSTROM);
}

void send_swell_signal(void)
{
	int id;

	id = port_shm_get_random_swell(state.ports, state.config);
	port_shm_send_signal_to_port(state.ports, id, SIGSWELL);
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
	config_shm_detach(state.config);
}
