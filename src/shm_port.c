#define _GNU_SOURCE

#include <string.h>
#include <signal.h>
#include <stdio.h>

#include "../lib/shm.h"
#include "../lib/semaphore.h"

#include "include/utils.h"
#include "include/const.h"
#include "include/shm_general.h"
#include "include/types.h"
#include "include/shm_port.h"

struct shm_port {
	pid_t pid;
	struct coord coord;
	int num_docks;

	bool_t is_in_swell;
	bool_t dump_had_swell;

	int dump_cargo_available;
	int dump_cargo_shipped;
	int dump_cargo_received;
	int sem_docks_id;
};

/* Ports shared memory */
shm_port_t *shm_port_initialize(shm_general_t *g)
{
	shm_port_t *ports;
	int shm_id, n_ports;
	size_t size;

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



	return ports;
}

void shm_port_ipc_init(shm_general_t *g, shm_port_t *p)
{
	int i, n_ports, n_docks, rand_docks, sem_docks_id;
	n_ports = get_porti(g);
	n_docks = get_banchine(g);

	/* Semaphores */
	sem_docks_id = sem_create(SEM_DOCK_KEY, n_ports);
	for (i = 0; i < n_ports; i++) {
		p[i].sem_docks_id = sem_docks_id;
		rand_docks = RANDOM_INTEGER(1,n_docks);
		sem_setval(p[i].sem_docks_id, i, rand_docks);
		p[i].num_docks = rand_docks;
	}
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

void shm_port_send_signal_to_port(shm_port_t *p, int port_id, int signal){kill(p[port_id].pid, signal);}

/* Setters */
void shm_port_set_pid(shm_port_t *p, int port_id, pid_t pid){p[port_id].pid = pid;}
void shm_port_set_coordinates(shm_port_t *p, int port_id, struct coord coord){p[port_id].coord = coord;}

void shm_port_set_is_in_swell(shm_port_t *p, int port_id, bool_t value)
{
	if (p[port_id].dump_had_swell == FALSE && value == TRUE)
		p[port_id].dump_had_swell = TRUE;
	p[port_id].is_in_swell = value;
}

void shm_port_update_dump_cargo_available(shm_general_t *g, shm_port_t *p, shm_offer_t *o, int port_id){p[port_id].dump_cargo_available = shm_offer_get_tot_quantity(g, o, port_id);}
void shm_port_update_dump_cargo_shipped(shm_port_t *p, int port_id, int amount){p[port_id].dump_cargo_shipped += amount;}
void shm_port_update_dump_cargo_received(shm_port_t *p, int port_id, int amount){p[port_id].dump_cargo_received += amount;}

/* Getters */
struct coord shm_port_get_coordinates(shm_port_t *p, int port_id){return p[port_id].coord;}
int shm_port_get_docks(shm_port_t *p, int port_id){return p[port_id].num_docks;}
int shm_port_get_sem_docks_id(shm_port_t *p){return p->sem_docks_id;}
pid_t shm_port_get_pid(shm_port_t *p, int port_id){return p[port_id].pid;}
int shm_port_get_dump_used_docks(shm_port_t *p, int port_id){return p[port_id].num_docks - sem_getval(p->sem_docks_id, port_id);}

int shm_port_get_dump_had_swell(shm_port_t *p, int n_ports)
{
	int id, cnt = 0;
	for(id = 0; id < n_ports; id++)
		if(p[id].dump_had_swell == TRUE)
			cnt++;
	return cnt;
}

bool_t shm_port_get_dump_having_swell(shm_port_t *p, int port_id){return p[port_id].is_in_swell;}
bool_t shm_port_get_dump_swell_final(shm_port_t *p, int port_id){return p[port_id].dump_had_swell;}

int shm_port_get_dump_cargo_available(shm_port_t *p, int port_id){return p[port_id].dump_cargo_available;}
int shm_port_get_dump_cargo_shipped(shm_port_t *p, int port_id){return p[port_id].dump_cargo_shipped;}
int shm_port_get_dump_cargo_received(shm_port_t *p, int port_id){return p[port_id].dump_cargo_received;}

void shm_port_remove_expired(shm_general_t *g, shm_port_t *p, shm_offer_t *o, shm_cargo_t *c, o_list_t **cargo_hold, int port_id)
{
	int i, removed, sem_cargo_id;
	sem_cargo_id = sem_cargo_get_id(g);

	for (i = 0; i < get_merci(g); i++) {
		removed = cargo_list_remove_expired(cargo_hold[i], get_current_day(g));
		if (removed > 0){
			shm_offer_remove_quantity(o, g, port_id, i, removed);
			shm_cargo_update_dump_available_in_port(c, i, -removed, sem_cargo_id);
			shm_cargo_update_dump_expired_in_port(c, i, removed, sem_cargo_id);
		}
	}
}
