/**
 * @file semaphore.h
 * @brief Library that provides a more user friendly interface for System V semaphores.
 */

#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include <sys/sem.h>

/**
 * @brief Create a new semaphore array and returns the id. Initializes the permissions to 0660.
 *
 * @param sem_key semaphore key, if 0 IPC_PRIVATE flag is used.
 * @param nsems number of semaphores in the array.
 * @return the id of the semaphore array. Returns -1 on failure.
 */
id_t sem_create(key_t sem_key, int nsems);

/**
 * @brief Gets the id of an existing semaphore set.
 *
 * @param key the key of the set.
 * @return the id of the set. Returns -1 on failure.
 */
int sem_get_id(key_t key)

/** @brief Sets the initial value of a semaphore in the array.
 *
 * @param sem_id the id of the semaphore array.
 * @param sem_index the index of the semaphore in the array.
 * @param value the value to set.
 */
void sem_setval(id_t sem_id, int sem_index, int value);

/**
 * @brief Sets the initial value of a semaphore in the array.
 *
 * @param sem_id the id of the semaphore array.
 * @param sem_index the index of the semaphore in the array.
 * @param op_val the operation performed on the semaphore.
 * @param flags the flags used for the operation. Use 0 for no flags.
 */
void sem_execute_semop(id_t sem_id, int sem_index, int op_val, int flags);

/**
 * @brief Deletes a semaphore array.
 *
 * @param sem_id the id of the array that will be deleted.
 */
void sem_delete(id_t sem_id);


#endif
