#ifndef OS_PROJECT_SHM_PORT_H
#define OS_PROJECT_SHM_PORT_H

#include <sys/types.h>

#include "shm_general.h"
#include "shm_cargo.h"
#include "shm_offer_demand.h"
#include "cargo_list.h"
#include "types.h"

/**
 * @brief Represents the shared memory structure for port information.
 */
typedef struct shm_port shm_port_t;

/**
 * @brief Initializes and attaches shared memory for port data.
 * @param g Pointer to the general shared memory structure.
 * @return Pointer to the attached port data structure or NULL on failure.
 */
shm_port_t *shm_port_initialize(shm_general_t *g);

/**
 * @brief Initializes ipc related to port shm.
 * @param g Pointer to general shared memory structure.
 * @param p Pointer to port share memory structure.
 */
void shm_port_ipc_init(shm_general_t *g, shm_port_t *p);

/**
 * @brief Attaches the process to the shared memory segment for port data.
 * @param g Pointer to the general shared memory structure.
 * @return Pointer to the attached port data structure or NULL on failure.
 */
shm_port_t *shm_port_attach(shm_general_t *g);

/**
 * @brief Detaches the process from the shared memory segment for port data.
 * @param p Pointer to the array of port data in shared memory.
 */
void shm_port_detach(shm_port_t *p);

/**
 * @brief Deletes the shared memory segment for port data.
 * @param g Pointer to the general shared memory structure.
 */
void shm_port_delete(shm_general_t *g);

/**
* @brief Sends a signal to all ports in the shared memory structure.
* @param p Pointer to the array of port data in shared memory.
* @param g Pointer to the general shared memory structure.
* @param signal Signal to send.
*/
void shm_port_send_signal_to_all_ports(shm_port_t *p, shm_general_t *g,
				       int signal);

/**
 * @brief Sends a signal to a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param port_id Identifier of the port.
 * @param signal Signal to send.
 */
void shm_port_send_signal_to_port(shm_port_t *p, int port_id, int signal);

/**
 * @brief Sets the process ID for a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param port_id Identifier of the port.
 * @param pid Process ID to set.
 */
void shm_port_set_pid(shm_port_t *p, int port_id, pid_t pid);

/**
 * @brief Sets the coordinates for a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param port_id Identifier of the port.
 * @param coord Coordinates to set.
 */
void shm_port_set_coordinates(shm_port_t *p, int port_id, struct coord coord);

/**
 * @brief Sets is_in_swell to value when port receive SIGSWELL
 * @param p Pointer to the array of port data in shared memory.
 * @param port_id Identifier of the port.
 * @param value the value to assign to is_in_swell
 */
void shm_port_set_is_in_swell(shm_port_t *p, int port_id, bool_t value);

/**
 * @brief Updates the number of available cargo at a specific port in the dump.
 * @param g Pointer to the shm_general_t structure.
 * @param p Pointer to the shm_port_t structure.
 * @param o Pointer to the shm_offer_t structure.
 * @param port_id The identifier of the port.
 */
void shm_port_update_dump_cargo_available(shm_general_t *g, shm_port_t *p, shm_offer_t *o, int port_id);

/**
 * @brief Updates the total shipped cargo at a specific port in the dump.
 * @param p Pointer to the shm_port_t structure.
 * @param port_id The identifier of the port.
 * @param amount The number of shipped cargo to add to the dump.
 */
void shm_port_update_dump_cargo_shipped(shm_port_t *p, int port_id, int amount);

/**
 * @brief Updates the total received cargo at a specific port in the dump.
 * @param p Pointer to the shm_port_t structure.
 * @param port_id The identifier of the port.
 * @param amount The number of received cargo to add to the dump.
 */
void shm_port_update_dump_cargo_received(shm_port_t *p, int port_id, int amount);

/**
 * @brief Gets the coordinates of a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param port_id Identifier of the port.
 * @return Coordinates of the specified port.
 */
struct coord shm_port_get_coordinates(shm_port_t *p, int port_id);

/**
 * @brief Gets the number of docks of a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param port_id Identifier of the port.
 */
int shm_port_get_docks(shm_port_t *p, int port_id);

/**
 * @brief Gets the semaphore identifier for docks at a specific port.
 * @param p Pointer to the shm_port_t structure.
 * @return The semaphore identifier for docks at the port.
 */
int shm_port_get_sem_docks_id(shm_port_t *p);

/* Dump getters */

/**
 * @brief Gets the number of used docks in the dump at a specific port.
 * @param p Pointer to the shm_port_t structure.
 * @param port_id The identifier of the port.
 * @return The number of used docks at the specified port.
 */
int shm_port_get_dump_used_docks(shm_port_t *p, int port_id);

/**
 * @brief Gets the count of ports that had a swell event in the dump.
 * @param p Pointer to the array of shm_port_t structures.
 * @param n_ports The total number of ports.
 * @return The count of ports that had a swell event in the dump.
 */
int shm_port_get_dump_had_swell(shm_general_t *g, shm_port_t *p);

/**
 * @brief Gets whether a specific port had a swell event in the dump.
 * @param p Pointer to the shm_port_t structure.
 * @param port_id The identifier of the port.
 * @return TRUE if the port had a swell event in the dump, FALSE otherwise.
 */
bool_t shm_port_get_dump_having_swell(shm_port_t *p, int port_id);

/**
 * @brief Gets the final swell status for a specific port in the dump.
 * @param p Pointer to the shm_port_t structure.
 * @param port_id The identifier of the port.
 * @return TRUE if the port had a swell event in the dump during entire simulation, FALSE otherwise.
 */
bool_t shm_port_get_dump_swell_final(shm_port_t *p, int port_id);

/**
 * @brief Gets the availability of cargo at a specific port in the dump.
 * @param p Pointer to the shm_port_t structure.
 * @param port_id The identifier of the port.
 * @return The number of available cargo at the specified port.
 */
int shm_port_get_dump_cargo_available(shm_port_t *p, int port_id);

/**
 * @brief Gets the total shipped cargo at a specific port in the dump.
 * @param p Pointer to the shm_port_t structure.
 * @param port_id The identifier of the port.
 * @return The total number of shipped cargo from the specified port.
 */
int shm_port_get_dump_cargo_shipped(shm_port_t *p, int port_id);

/**
 * @brief Gets the total received cargo at a specific port in the dump.
 * @param p Pointer to the shm_port_t structure.
 * @param port_id The identifier of the port.
 * @return The total number of received cargo at the specified port.
 */
int shm_port_get_dump_cargo_received(shm_port_t *p, int port_id);

/**
 * @brief Removes expired cargo from a port's cargo hold and updates related data structures.
 * @param g Pointer to the shm_general_t structure.
 * @param p Pointer to the shm_port_t structure.
 * @param o Pointer to the array of offer.
 * @param c Pointer to the shm_offer_t structures.
 * @param cargo_hold Array of cargo lists representing the cargo hold of each port.
 * @param port_id The identifier of the port.
 */
void shm_port_remove_expired(shm_general_t *g, shm_port_t *p, shm_offer_t *o, shm_cargo_t *c, o_list_t **cargo_hold, int port_id);

#endif
