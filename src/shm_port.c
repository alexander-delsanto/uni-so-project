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
	bool_t had_swell;
	int dump_cargo_available;
	int dump_cargo_shipped;
	int dump_cargo_received;
	int dump_used_docks;
	int dump_ships_arrived;
};

/* Ports shared memory */
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

	return ports;
}

shm_port_t *port_shm_attach(shm_general_t *c)
{
	shm_port_t *ports;

	ports = shm_attach(get_port_shm_id(c));

	return ports;
}

void port_shm_detach(shm_port_t *p)
{
	shm_detach(p);
}

void port_shm_delete(shm_general_t *c)
{
	shm_delete(get_port_shm_id(c));
}

/* Signals to ports */
void port_shm_send_signal_to_all_ports(shm_port_t *p, shm_general_t *c,
				       int signal)
{
	int i;
	int n_ships = get_porti(c);

	for (i = 0; i < n_ships; i++) {
		kill(p[i].pid, signal);
	}
}

void port_shm_send_signal_to_port(shm_port_t *p, int id, int signal){kill(p[id].pid, signal);}

/* Setters */
void port_shm_set_coordinates(shm_port_t *p, int id, struct coord coord){p[id].coord = coord;}
void port_shm_set_pid(shm_port_t *p, int id, pid_t pid){p[id].pid = pid;}
void port_shm_set_docks(shm_port_t *p, int id, int n){p[id].num_docks = n;}
void port_shm_set_dump_cargo_available(shm_port_t *p, int id, int n){p[id].dump_cargo_available += n;}
void port_shm_set_dump_cargo_shipped(shm_port_t *p, int id, int n){p[id].dump_cargo_shipped += n;}
void port_shm_set_dump_cargo_received(shm_port_t *p, int id, int n){p[id].dump_cargo_received += n;}
void port_shm_set_dump_used_docks(shm_port_t *p, int id, int n){p[id].dump_used_docks += n;}
void port_shm_set_dump_ships_arrived(shm_port_t *p, int id, int n){p[id].dump_ships_arrived += n;}

void port_shm_set_is_in_swell(shm_port_t *p, int id, bool_t value)
{
	if (p[id].had_swell == FALSE && value == TRUE)
		p[id].had_swell = TRUE;
	p[id].is_in_swell = value;
}


/* Getters */
struct coord port_shm_get_coords(shm_port_t *p, int id){return p[id].coord;}
int port_shm_get_docks(shm_port_t *p, int id){return p[id].num_docks;}
pid_t port_shm_get_pid(shm_port_t *p, int id){return p[id].pid;}
int port_shm_get_dump_cargo_available(shm_port_t *p, int id){return p[id].dump_cargo_available;}
int port_shm_get_dump_cargo_shipped(shm_port_t *p, int id){return p[id].dump_cargo_shipped;}
int port_shm_get_dump_cargo_received(shm_port_t *p, int id){return p[id].dump_cargo_received;}
int port_shm_get_dump_used_docks(shm_port_t *p, int id){return p[id].dump_used_docks;}
int port_shm_get_dump_ships_arrived(shm_port_t *p, int id){return p[id].dump_ships_arrived;}

int port_shm_get_had_swell(shm_port_t *p, int n_ports)
{
	int id, cnt = 0;
	for(id = 0; id < n_ports; id++)
		if(p[id].had_swell == TRUE)
			cnt++;
	return cnt;
}

bool_t port_shm_get_dump_swell_final(shm_port_t *p, int id){return p[id].had_swell;}

/* TODO: funzioni di distruzione della roba */

void port_shm_generate_cargo(shm_port_t *p, int id, shm_general_t *c)
{
	int *cargo_exp;
	/*cargo_exp = cargo_generate(c, p->cargo);*/
}
