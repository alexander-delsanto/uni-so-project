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
 * @brief Sets the process ID for a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @param pid Process ID to set.
 */
void port_shm_set_pid(shm_port_t *p, int id, pid_t pid);

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
 * @brief Sets is_in_swell to value when port receive SIGSWELL
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @param value the value to assign to is_in_swell
 */
void port_shm_set_is_in_swell(shm_port_t *p, int id, bool_t value);

/**
 * @brief Gets the coordinates of a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @return Coordinates of the specified port.
 */
struct coord port_shm_get_coords(shm_port_t *p, int id);

/**
 * @brief Gets the number of docks of a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 */
int port_shm_get_docks(shm_port_t *p, int id);

/**
 * @brief Gets the process ID of a specific port in the shared memory structure.
 * @param p Pointer to the array of port data in shared memory.
 * @param id Identifier of the port.
 * @return Process ID of the specified port.
 */
pid_t port_shm_get_pid(shm_port_t *p, int id);

/* Dump setters */
void port_shm_set_dump_cargo_available(shm_port_t *p, int id, int n);
void port_shm_set_dump_cargo_shipped(shm_port_t *p, int id, int n);
void port_shm_set_dump_cargo_received(shm_port_t *p, int id, int n);
void port_shm_set_dump_used_docks(shm_port_t *p, int id, int n);
void port_shm_set_dump_ships_arrived(shm_port_t *p, int id, int n);

/* Dump getters */
int port_shm_get_dump_cargo_available(shm_port_t *p, int id);
int port_shm_get_dump_cargo_shipped(shm_port_t *p, int id);
int port_shm_get_dump_cargo_received(shm_port_t *p, int id);
int port_shm_get_dump_used_docks(shm_port_t *p, int id);
int port_shm_get_dump_ships_arrived(shm_port_t *p, int id);
int port_shm_get_dump_had_swell(shm_port_t *p, int id);
bool_t port_shm_get_dump_having_swell(shm_port_t *p, int id);
bool_t port_shm_get_dump_swell_final(shm_port_t *p, int id);

void port_shm_dump_expired_add(shm_port_t *p, int port_id, int id,
			       int quantity);
void port_shm_dump_received_add(shm_port_t *p, int port_id, int id,
				int quantity);
void port_shm_present_add(shm_port_t *p, int port_id, int id, int quantity);
void port_shm_dump_sent_add(shm_port_t *p, int port_id, int id, int quantity);
#endif
