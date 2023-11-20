#define _GNU_SOURCE

#include <sys/time.h>
#include "header/utils.h"
#include "header/shared_memory.h"
#include "header/ipc_utils.h"

void send_storm_signal(void);
void send_swell_signal(void);
void start_timer(double timer_interval);
void signal_handler(int signal);
void close_all(void);

int main(int argc, char *argv[])
{
	struct sigaction sa;
	sigset_t mask;
	bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_handler;

	/* Signal handler initialization */
	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);

	/* Attaching to shared memory */
	attach_process_to_shm();

	srand(getpid() * time(NULL));
	sigemptyset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGALRM, &sa, NULL);

	start_timer(SO_MAELSTROM / 24.0);

	while (1) {
		pause();
	}
}

void send_storm_signal(void)
{
	int i, target_ship = RANDOM_INTEGER(0, (SO_NAVI - 1));

	for (i = 0; i < SO_NAVI; i++) {
		if (!get_ship_is_dead(target_ship) &&
		    get_ship_is_moving(target_ship)) {
			kill(get_ship_pid(target_ship), SIGSTORM);
			return;
		}
		target_ship = (target_ship + 1) % SO_NAVI;
	}
}

void send_maelstrom_signal(void)
{
	int i, target_ship = RANDOM_INTEGER(0, (SO_NAVI - 1));
	for (i = 0; i < SO_NAVI; i++) {
		if (!get_ship_is_dead(target_ship)) {
			kill(get_ship_pid(target_ship), SIGMAELSTROM);
			return;
		}
		target_ship = (target_ship + 1) % SO_NAVI;
	}
}

void send_swell_signal(void)
{
	int target_port = RANDOM_INTEGER(0, (SO_PORTI - 1));
	kill(target_port, SIGSTORM);
}

void start_timer(double timer_interval)
{
	struct itimerval timer;
	int sec, usec;

	sec = (int)timer_interval;
	usec = (timer_interval - sec) * 1e6;

	timer.it_value.tv_sec = sec;
	timer.it_value.tv_usec = usec;
	timer.it_interval.tv_sec = sec;
	timer.it_interval.tv_usec = usec;

	setitimer(ITIMER_REAL, &timer, NULL);
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
		send_storm_signal();
		/*send_swell_signal();*/
		break;
	case SIGALRM:
		send_maelstrom_signal();
		break;
	case SIGSEGV:
		dprintf(2, "weather.c: Segmentation fault. Closing.\n");
	case SIGINT:
		close_all();
	}
}

void close_all(void)
{
	detach_all_shm();
	exit(0);
}
