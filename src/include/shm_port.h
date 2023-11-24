#ifndef OS_PROJECT_SHM_PORT_H
#define OS_PROJECT_SHM_PORT_H

#include <sys/types.h>

#include "shm_general.h"
#include "types.h"

typedef struct shm_port shm_port_t;

/**
 * @brief Initializes and attaches shared memory for port data.
 * @param g Pointer to the general shared memory structure.
 * @return Pointer to the attached port data structure or NULL on failure.
 */
shm_port_t *port_initialize(shm_general_t *g);
/**
 * @brief Attaches the process to the shared memory segment for port data.
 * @param c Pointer to the general shared memory structure.
 * @return Pointer to the attached port data structure or NULL on failure.
 */
shm_port_t *port_shm_attach(shm_general_t *c);
/**
 * @brief Detaches the process from the shared memory segment for port data.
 * @param p Pointer to the array of port data in shared memory.
 */
void port_shm_detach(shm_port_t *p);
/**
 * @brief Deletes the shared memory segment for port data.
 * @param c Pointer to the general shared memory structure.
 */
void port_shm_delete(shm_general_t *c);

/**
 * @brief Sets the process ID for a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @param pid Process ID to set.
 */
void port_shm_set_pid(shm_port_t *p, int id, pid_t pid);
/**
* @brief Sends a signal to all ports in the shared memory structure.
* @param p Pointer to the array of port data in shared memory.
* @param c Pointer to the general shared memory structure.
* @param signal Signal to send.
*/
void port_shm_send_signal_to_all_ports(shm_port_t *p, shm_general_t *c,
				       int signal);
/**
 * @brief Sends a signal to a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @param signal Signal to send.
 */
void port_shm_send_signal_to_port(shm_port_t *p, int id, int signal);

/**
 * @brief Sets the coordinates for a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @param coord Coordinates to set.
 */
void port_shm_set_coordinates(shm_port_t *p, int id, struct coord coord);

/**
 * @brief Sets the number of docks for a specific port.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @param n Number of docks to set.
 */
void port_shm_set_docks(shm_port_t *p, int id, int n);

/**
 * @brief Gets the coordinates of a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @return Coordinates of the specified port.
 */
struct coord port_shm_get_coords(shm_port_t *p, int id);

/**
 * @brief Gets the process ID of a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @return Process ID of the specified port.
 */
pid_t port_shm_get_pid(shm_port_t *p, int id);

#endif
