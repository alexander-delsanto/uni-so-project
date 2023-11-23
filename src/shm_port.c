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

	key_t sem_id;

	//cargo_t *cargo;

	bool_t had_swell;
	int dump_cargo_available;
	int dump_cargo_shipped;
	int dump_cargo_received;
	int dump_used_docks;
	int dump_ships_arrived;
	bool_t dump_had_swell;
};

shm_port_t *port_initialize(shm_general_t *g)
{
	shm_port_t *ports;
	int i;
	int shm_id, n_ports;
	size_t size;

	n_ports = get_porti(g);

	size = sizeof(shm_port_t) * n_ports;

	shm_id = shm_create(SHM_DATA_PORTS_KEY, size);
	if (shm_id == -1) {
		return NULL;
	}

	ports = shm_attach(shm_id);
	bzero(ports, size);
	set_port_shm_id(g, shm_id);

	ports->sem_id = sem_create(SEM_PORT_KEY, n_ports);
	for (i = 0; i < n_ports; i++) {
		sem_setval(ports->sem_id, i, 1);
	}

	return ports;
}

shm_port_t *port_shm_attach(shm_general_t *c)
{
	shm_port_t *ports;

	ports = shm_attach(get_port_shm_id(c));

	return ports;
}

void port_shm_set_docks(shm_port_t *p, int id, int n)
{
	p[id].num_docks = n;
}

void port_shm_generate_cargo(shm_port_t *p, int id, shm_general_t *c)
{
	int *cargo_exp;
	/*cargo_exp = cargo_generate(c, p->cargo);*/
}

void port_shm_detach(shm_port_t *p)
{
	shm_detach(p);
}

void port_shm_delete(shm_general_t *c)
{
	shm_delete(get_port_shm_id(c));
}

void port_shm_set_coordinates(shm_port_t *p, int id, struct coord coord)
{
	p[id].coord = coord;
}

void port_shm_set_pid(shm_port_t *p, int id, pid_t pid)
{
	p[id].pid = pid;
}

void port_shm_send_signal_to_all_ports(shm_port_t *p, shm_general_t *c,
				       int signal)
{
	int i;
	int n_ships = get_porti(c);

	for (i = 0; i < n_ships; i++) {
		kill(p[i].pid, signal);
	}
}

void port_shm_send_signal_to_port(shm_port_t *p, int id, int signal)
{
	kill(p[id].pid, signal);
}

struct coord port_shm_get_coords(shm_port_t *p, int id)
{
	return p[id].coord;
}

pid_t port_shm_get_pid(shm_port_t *p, int id)
{
	return p[id].pid;
}

/* TODO: funzioni di distruzione della roba */