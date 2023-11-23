#define _GNU_SOURCE

#include "include/sem.h"
#include "../lib/semaphore.h"

int get_sem_start_id(void)
{
	return sem_get_id(SEM_START_KEY);
}

int get_sem_port_init_id(void)
{
	return sem_get_id(SEM_PORTS_INITIALIZED_KEY);
}

int get_sem_port_id(void)
{
	return sem_get_id(SEM_PORT_KEY);
}
