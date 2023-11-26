#ifndef OS_PROJECT_SHM_SHIP_H
#define OS_PROJECT_SHM_SHIP_H

#include "shm_general.h"

typedef struct shm_ship shm_ship_t;

/**
 * @brief Initializes and attaches shared memory for ship data.
 * @param c Pointer to the general shared memory structure.
 * @return Pointer to the attached ship data structure or NULL on failure.
 */
shm_ship_t *ship_initialize(shm_general_t *c);
/**
 * @brief Attaches the process to the shared memory segment for ship data.
 * @param c Pointer to the general shared memory structure.
 * @return Pointer to the attached ship data structure or NULL on failure.
 */
shm_ship_t *ship_shm_attach(shm_general_t *c);
/**
 * @brief Detaches the process from the shared memory segment for ship data.
 * @param c Pointer to the array of ship data in shared memory.
 */
void ship_shm_detach(shm_ship_t *c);
/**
 * @brief Deletes the shared memory segment for ship data.
 * @param c Pointer to the general shared memory structure.
 */
void ship_shm_delete(shm_general_t *c);

/**
 * @brief Sends a signal to all ships in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param c Pointer to the general shared memory structure.
 * @param signal Signal to send.
 */
void ship_shm_send_signal_to_all_ships(shm_ship_t *s, shm_general_t *c,
				       int signal);
/**
 * @brief Sends a signal to a specific ship in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 * @param signal Signal to send.
 */
void ship_shm_send_signal_to_ship(shm_ship_t *s, int id, int signal);


/**
 * @brief Sets the process ID for a specific ship in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 * @param pid Process ID to set.
 */
void ship_shm_set_pid(shm_ship_t *s, int id, pid_t pid);
/**
 * @brief Sets the coordinates for a specific ship in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 * @param coords Coordinates to set.
 */
void ship_shm_set_coords(shm_ship_t *s, int id, struct coord coords);
/**
 * @brief Marks a specific ship as dead in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 */
void ship_shm_set_is_dead(shm_ship_t *s, int id);
/**
 * @brief Sets the "is_moving" status for a specific ship in the shared memory structure
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 */
void ship_shm_set_is_moving(shm_ship_t *s, int id, bool_t value);

/* Dump setters */
void ship_shm_set_dump_with_cargo(shm_ship_t *s, int id, bool_t value);
void ship_shm_set_dump_had_storm(shm_ship_t *s, int id);

/**
 * @brief Gets the process ID of a specific ship in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 * @return Process ID of the specified ship.
 */
pid_t ship_shm_get_pid(shm_ship_t *s, int id);
/**
 * @brief Gets the "is_dead" status of a specific ship in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 * @return "is_dead" status of the specified ship.
 */
bool_t ship_shm_get_dead(shm_ship_t *s, int id);
/**
 * @brief Gets the "is_moving" status of a specific ship in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 * @return "is_moving" status of the specified ship.
 */
bool_t ship_shm_get_is_moving(shm_ship_t *s, int id);
/**
 * @brief Gets the coordinates of a specific ship in the shared memory structure.
 * @param s Pointer to the array of ship data in shared memory.
 * @param id Identifier of the ship.
 * @return Coordinates of the specified ship.
 */
struct coord ship_shm_get_coords(shm_ship_t *s, int id);

/* Dump getters */
int ship_shm_get_dump_with_cargo(shm_ship_t *s, int n_ships);
int ship_shm_get_dump_without_cargo(shm_ship_t *s, int n_ships);
int ship_shm_get_dump_on_port(shm_ship_t *s, int n_ships);
int ship_shm_get_dump_had_storm(shm_ship_t *s, int n_ships);
int ship_shm_get_dump_had_maelstrom(shm_ship_t *s, int n_ships);
int ship_shm_get_dump_is_dead(shm_ship_t *s, int n_ships);

#endif
