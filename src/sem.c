#define _GNU_SOURCE

#include "include/sem.h"
#include "../lib/semaphore.h"

int get_sem_start_id(void)
{
	return sem_create(SEM_START_KEY, 1);
}

int get_sem_port_init_id(void)
{
	return sem_create(SEM_PORTS_INITIALIZED_KEY, 1);
}
