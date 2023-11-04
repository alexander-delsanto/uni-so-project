#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include "../lib/semaphore.h"
#include "../src/header/ipc_utils.h"

#define SEM_KEY 0x666666
#define NUM_CONST 16

int id_sem;

void reader();
void ship(int id);

int main()
{
	struct data_general *general;
	int i, status;
	pid_t *pid_ships, pid_reader;

	general = malloc(sizeof(*general));
	general->so_lato = 1.0;
	general->so_days = 5;
	general->so_navi = 100;
	general->so_speed = 60;
	general->so_capacity = 8;
	general->so_porti = 4;
	general->so_banchine = 3;
	general->so_fill = 3;
	general->so_loadspeed = 15;
	general->so_merci = 10;
	general->so_size = 10;
	general->so_min_vita = 3;
	general->so_max_vita = 8;
	general->so_storm_duration = 1;
	general->so_swell_duration = 1;
	general->so_maelstrom = 0;

	initialize_general_shm(general);
	initialize_shm();

	pid_ships = malloc(sizeof(*pid_ships) * SO_NAVI);
	for (i = 0; i < SO_NAVI; i++) {
		if ((pid_ships[i] = fork()) == 0) {
			ship(i);
		}
		set_ship_pid(i, pid_ships[i]);
	}

	id_sem = sem_create(SEM_KEY, 1);
	// sem_setval(id_sem, 0, SO_NAVI);
	sem_setval(id_sem, 0, 1);


	if ((pid_reader = fork()) == 0) {
		reader();
		exit(EXIT_SUCCESS);
	}

	sem_execute_semop(id_sem, 0, 0, 0);

	kill(pid_reader, SIGKILL);
	for (i = 0; i < SO_NAVI; i++) {
		kill(pid_ships[i], SIGKILL);
	}

	delete_all_shm();
	sem_delete(id_sem);
	exit(EXIT_SUCCESS);
}


void reader()
{
	int i;
	struct data_ship *ships;
	ships = shm_attach(get_ship_shm_id());

	for (i = 0; i < SO_NAVI; i++) {
		dprintf(1, "Ship %d: pid: %d, x: %f, y: %f.\n", i, ships[i].pid, ships[i].coord.x, ships[i].coord.y);
		//sem_execute_semop(id_sem, 0, -1, 0);
	}

	sem_execute_semop(id_sem, 0, -1, 0);
	shm_detach((void *)ships);
}

void ship(int id)
{
	srand(getpid() * time(NULL));
	struct coordinates ship_coords;
	ship_coords.x = RANDOM_DOUBLE(0, SO_LATO);
	ship_coords.y = RANDOM_DOUBLE(0, SO_LATO);
	set_ship_coords(id, ship_coords);

	sleep(100);
}
