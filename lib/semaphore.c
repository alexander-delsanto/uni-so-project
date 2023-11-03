#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include "semaphore.h"

/* Private functions prototypes */
static struct sembuf create_sembuf(int index, int semop_value, int flags);

id_t sem_create(key_t sem_key, int nsems)
{
	int res;
	if (( res = semget(sem_key, nsems, 0660 | IPC_CREAT)) < 0) {
		dprintf(2, "semaphore.c - sem_create: Failed to create semaphore array.\n");
		exit(EXIT_FAILURE);
	}
	return res;
}

void sem_setval(id_t sem_id, int sem_index, int value)
{
	if (semctl(sem_id, sem_index, SETVAL, value) < 0) {
		dprintf(2, "semaphore.c - sem_setval: Failed to set semaphore value\n");
		exit(EXIT_FAILURE);
	}
}

void sem_execute_semop(id_t sem_id, int sem_index, int op_val, int flags)
{
	struct sembuf operation;

	operation = create_sembuf(sem_index, operation, flags);
	while(semop(sem_id, &operation, 1) == -1);
}

void sem_delete(id_t sem_id)
{
	if (semctl(sem_id, 0, IPC_RMID) < 0) {
		dprintf(2, "semaphore.c - sem_delete: Failed to delete semaphore set.\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Private function that creates sembuf struct with given values.
 *
 * @param index index of the semaphore in the array.
 * @param semop_value semaphore operation.
 * @param flags operation flags.
 * @return the sembuf struct containing the given values.
 */
static struct sembuf create_sembuf(int index, int semop_value, int flags)
{
	struct sembuf res;
	res.sem_num = index;
	res.sem_op = semop_value;
	res.sem_flg = flags;

	return res;
}

