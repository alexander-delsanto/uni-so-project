#define _GNU_SOURCE
#include "header/shared_memory.h"

id_shm_t get_shm(key_t shm_key, size_t size)
{
	return shmget(shm_key, size, IPC_CREAT | 0600);
}

void *attach_shm(id_shm_t shm_id, int flags)
{
	return shmat(shm_id, NULL, flags);
}

void detach_shm(void *shm_ptr)
{
	if(shm_ptr != NULL)
		shmdt(shm_ptr);
}
