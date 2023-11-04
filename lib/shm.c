#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shm.h"

int shm_create(key_t key, size_t size)
{
	int res;
	if ((res = shmget(key, size, 0660 | IPC_CREAT)) == -1) {
		dprintf(2, "shm.c - shm_create() : Failed to create SHM segment.\n");
		perror("shmget");
	}
	return res;
}

void shm_delete(int id_shm)
{
	if (shmctl(id_shm, IPC_RMID, NULL) == -1) {
		dprintf(2, "shm.c - shm_delete() : Failed to delete SHM segment.\n");
		perror("shmctl");
	}
}

void *shm_attach(int id_shm)
{
	void *res;
	if ((res =shmat(id_shm, NULL, 0)) == ((void *) -1)){
		dprintf(2, "shm.c - shm_attach() : Failed to attach SHM segment.\n");
		perror("shmat");
	}
	return res;
}

void shm_detach(void *shm_ptr)
{
	if (shmdt(shm_ptr) == -1) {
		dprintf(2, "shm.c - shm_detach() : Failed to detach SHM segment.\n");
		perror("shmdt");
	}
}
