/**
* @file shm.h
* @brief Library that provides a more user friendly interface for System V shared memory.
*/

#ifndef SHM_H
#define SHM_H

#include <sys/shm.h>
#include <sys/stat.h>

/**
* @brief Creates a new shared memory segment.
*
* @param key the key for the shared memory.
* @param size the size of the segment.
* @return the id of the created segment. If segment exists with key the id of the existing segment.
*/
int shm_create(key_t key, size_t size);

/**
* @brief Deletes the shared memory segment.
*
* @param id_shm the id of the shared memory segment.
*/
void shm_delete(int id_shm);

/**
* @brief Attaches a shared memory segment.
*
* @param id_shm the id of the shared memory segment.
* @return
*/
void *shm_attach(int id_shm);

/**
* @brief Detaches a shared memory segment.
*
* @param shm_ptr the pointed to the segment.
*/
void shm_detach(void *shm_ptr);

#endif
