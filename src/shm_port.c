#define _GNU_SOURCE

#include <string.h>
#include <signal.h>
#include <stdio.h>

#include "../lib/shm.h"
#include "../lib/semaphore.h"

#include "include/const.h"
#include "include/shm_general.h"
#include "include/types.h"
#include "include/shm_port.h"

struct shm_port {
	pid_t pid;
	struct coord coord;
	int num_docks;

	int sem_id;

	bool_t is_in_swell;

	bool_t dump_had_swell;

	int dump_cargo_available;
	int dump_cargo_shipped;
	int dump_cargo_received;
	int dump_used_docks;
	int dump_ships_arrived;
};

/* Ports shared memory */
shm_port_t *shm_port_initialize(shm_general_t *g)
{
	shm_port_t *ports;
	int shm_id, n_ports;
	size_t size;

	dprintf(1, "port init\n");
	n_ports = get_porti(g);

	size = (sizeof(shm_port_t) + 4 * (sizeof(int) * get_merci(g))) *
	       n_ports;

	shm_id = shm_create(SHM_DATA_PORTS_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	ports = shm_attach(shm_id);
	bzero(ports, size);
	shm_port_set_id(g, shm_id);

	ports->sem_id = sem_create(SEM_PORT_KEY, n_ports);

	return ports;
}

shm_port_t *shm_port_attach(shm_general_t *g)
{
	shm_port_t *ports;

	ports = shm_attach(shm_port_get_id(g));

	return ports;
}

void shm_port_detach(shm_port_t *p)
{
	shm_detach(p);
}

void shm_port_delete(shm_general_t *g)
{
	shm_delete(shm_port_get_id(g));
}

/* Signals to ports */
void shm_port_send_signal_to_all_ports(shm_port_t *p, shm_general_t *g,
				       int signal)
{
	int i;
	int n_ships = get_porti(g);

	for (i = 0; i < n_ships; i++) {
		kill(p[i].pid, signal);
	}
}

void shm_port_send_signal_to_port(shm_port_t *p, int id, int signal){kill(p[id].pid, signal);}

/* Setters */
void shm_port_set_pid(shm_port_t *p, int id, pid_t pid){p[id].pid = pid;}
void shm_port_set_coordinates(shm_port_t *p, int id, struct coord coord){p[id].coord = coord;}
void shm_port_set_docks(shm_port_t *p, int id, int n){p[id].num_docks = n;}

void shm_port_set_is_in_swell(shm_port_t *p, int id, bool_t value)
{
	if (p[id].dump_had_swell == FALSE && value == TRUE)
		p[id].dump_had_swell = TRUE;
	p[id].is_in_swell = value;
}

void shm_port_set_dump_used_docks(shm_port_t *p, int id, int n){p[id].dump_used_docks += n;}
void shm_port_set_dump_ships_arrived(shm_port_t *p, int id, int n){p[id].dump_ships_arrived += n;}

void shm_port_set_dump_cargo_available(shm_port_t *p, int id, int n){p[id].dump_cargo_available += n;}
void shm_port_set_dump_cargo_shipped(shm_port_t *p, int id, int n){p[id].dump_cargo_shipped += n;}
void shm_port_set_dump_cargo_received(shm_port_t *p, int id, int n){p[id].dump_cargo_received += n;}

/* Getters */
struct coord shm_port_get_coordinates(shm_port_t *p, int id){return p[id].coord;}
int shm_port_get_docks(shm_port_t *p, int id){return p[id].num_docks;}
pid_t shm_port_get_pid(shm_port_t *p, int id){return p[id].pid;}
int shm_port_get_dump_used_docks(shm_port_t *p, int id){return p[id].dump_used_docks;}
int shm_port_get_dump_ships_arrived(shm_port_t *p, int id){return p[id].dump_ships_arrived;}

int shm_port_get_dump_had_swell(shm_port_t *p, int n_ports)
{
	int id, cnt = 0;
	for(id = 0; id < n_ports; id++)
		if(p[id].dump_had_swell == TRUE)
			cnt++;
	return cnt;
}

bool_t shm_port_get_dump_having_swell(shm_port_t *p, int id){return p[id].is_in_swell;}
bool_t shm_port_get_dump_swell_final(shm_port_t *p, int id){return p[id].dump_had_swell;}

int shm_port_get_dump_cargo_available(shm_port_t *p, int id){return p[id].dump_cargo_available;}
int shm_port_get_dump_cargo_shipped(shm_port_t *p, int id){return p[id].dump_cargo_shipped;}
int shm_port_get_dump_cargo_received(shm_port_t *p, int id){return p[id].dump_cargo_received;}