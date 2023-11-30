#ifndef OS_PROJECT_SHM_CARGO_H
#define OS_PROJECT_SHM_CARGO_H

#include <sys/types.h>

#include "shm_general.h"
#include "types.h"

/**
 * @brief Represents the shared memory structure for cargo information.
 */
typedef struct shm_cargo shm_cargo_t;

/* Cargo shm */

/**
 * @brief Initializes and returns the shared memory for cargo.
 *
 * @param g Pointer to shared memory general information.
 * @return Pointer to shared memory for cargo.
 */
shm_cargo_t *shm_cargo_initialize(shm_general_t *g);

/**
 * @brief Attaches to the existing shared memory for cargo.
 *
 * @param g Pointer to shared memory general information.
 * @return Pointer to shared memory for cargo.
 */
shm_cargo_t *shm_cargo_attach(shm_general_t *g);

/**
 * @brief Detaches from the shared memory for cargo.
 *
 * @param c Pointer to shared memory for cargo.
 */
void shm_cargo_detach(shm_cargo_t *c);

/**
 * @brief Deletes the shared memory for cargo.
 *
 * @param c Pointer to shared memory general information.
 */
void shm_cargo_delete(shm_general_t *c);

/* Getters */

/**
 * @brief Gets the size of the cargo batch for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @return Size of the cargo batch.
 */
int shm_cargo_get_size(shm_cargo_t *c, int id);

/**
 * @brief Gets the life span of the cargo batch for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @return Life span of the cargo batch.
 */
int shm_cargo_get_life(shm_cargo_t *c, int id);

/**
 * @brief Gets the ID of the cargo type with the minimum batch size.
 *
 * @param c Pointer to shared memory for cargo.
 * @param g Pointer to shared memory general information.
 * @return ID of the cargo type with the minimum batch size.
 */
int shm_cargo_get_min_size_id(shm_cargo_t *c, shm_general_t *g);

/**
 * @brief Gets the total quantity of generated cargo for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @return Total quantity of generated cargo.
 */
int shm_cargo_get_dump_total_generated(shm_cargo_t *c, int id);

/**
 * @brief Gets the quantity of expired cargo in the port for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @return Quantity of expired cargo in the port.
 */
int shm_cargo_get_dump_expired_in_port(shm_cargo_t *c, int id);

/**
 * @brief Gets the quantity of expired cargo on the ship for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @return Quantity of expired cargo on the ship.
 */
int shm_cargo_get_dump_expired_on_ship(shm_cargo_t *c, int id);

/**
 * @brief Gets the quantity of received cargo in the port for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @return Quantity of received cargo in the port.
 */
int shm_cargo_get_dump_received_in_port(shm_cargo_t *c, int id);

/**
 * @brief Gets the quantity of available cargo in the port for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @return Quantity of available cargo in the port.
 */
int shm_cargo_get_dump_available_in_port(shm_cargo_t *c, int id);

/**
 * @brief Gets the quantity of available cargo on the ship for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @return Quantity of available cargo on the ship.
 */
int shm_cargo_get_dump_available_on_ship(shm_cargo_t *c, int id);

/* Setters */

/**
 * @brief Sets the total quantity of generated cargo for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @param quantity Quantity to add.
 * @param sem_cargo_id Semaphore ID for the cargo type.
 */
void shm_cargo_update_dump_total_generated(shm_cargo_t *c, int id, int quantity, int sem_cargo_id);

/**
 * @brief Sets the quantity of expired cargo in the port for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @param quantity Quantity to set.
 * @param sem_cargo_id Semaphore ID for the cargo type.
 */
void shm_cargo_update_dump_expired_in_port(shm_cargo_t *c, int id, int quantity, int sem_cargo_id);

/**
 * @brief Sets the quantity of expired cargo on the ship for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @param quantity Quantity to set.
 * @param sem_cargo_id Semaphore ID for the cargo type.
 */
void shm_cargo_update_dump_expired_on_ship(shm_cargo_t *c, int id, int quantity, int sem_cargo_id);

/**
 * @brief Sets the quantity of received cargo in the port for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @param quantity Quantity to set.
 * @param sem_cargo_id Semaphore ID for the cargo type.
 */
void shm_cargo_update_dump_received_in_port(shm_cargo_t *c, int id, int quantity, int sem_cargo_id);

/**
 * @brief Sets the quantity of available cargo in the port for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @param quantity Quantity to set.
 * @param sem_cargo_id Semaphore ID for the cargo type.
 */
void shm_cargo_update_dump_available_in_port(shm_cargo_t *c, int id, int quantity, int sem_cargo_id);

/**
 * @brief Sets the quantity of available cargo on the ship for a specific cargo type.
 *
 * @param c Pointer to shared memory for cargo.
 * @param id Cargo type ID.
 * @param quantity Quantity to set.
 * @param sem_cargo_id Semaphore ID for the cargo type.
 */
void shm_cargo_update_dump_available_on_ship(shm_cargo_t *c, int id, int quantity, int sem_cargo_id);

#endif
