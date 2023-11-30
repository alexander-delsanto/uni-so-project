#ifndef OS_PROJECT_SHM_GENERAL_H
#define OS_PROJECT_SHM_GENERAL_H

/**
 * @brief Structure for storing general simulation parameters and shared memory identifiers.
 */
typedef struct shm_general shm_general_t;

/**
 * @brief Reads configuration values from a file and updates the shared memory structure.
 * @param path Path to the configuration file.
 * @param g Pointer to the pointer of the general shared memory structure.
 * @return Pointer to the updated general shared memory structure or NULL on failure.
 */
shm_general_t *read_from_path(char *path, shm_general_t **g);

/**
 * @brief Initializes ipc related to general shm.
 * @param g pointer to general shm struct.
 */
void shm_general_ipc_init(shm_general_t *g);

/**
 * @brief Attaches the process to the shared memory segment for the general structure.
 * @param g Pointer to the pointer of the general shared memory structure.
 */
void shm_general_attach(shm_general_t **g);

/**
 * @brief Detaches the process from the shared memory segment for the general structure.
 * @param g Pointer to the general shared memory structure.
 */
void shm_general_detach(shm_general_t *g);

/**
 * @brief Deletes the shared memory segment with the specified ID.
 * @param id Shared memory ID to be deleted.
 */
void shm_general_delete(int id);

/* SHM id setters */

/**
 * @brief Sets the shared memory ID for the ship information structure.
 * @param g  Pointer to the shm_general_t structure.
 * @param id The shared memory ID to be set for ships.
 */
void shm_ship_set_id(shm_general_t *g, int id);

/**
 * @brief Sets the shared memory ID for the port information structure.
 * @param g  Pointer to the shm_general_t structure.
 * @param id The shared memory ID to be set for ports.
 */
void shm_port_set_id(shm_general_t *g, int id);

/**
 * @brief Sets the shared memory ID for the cargo information structure.
 * @param g  Pointer to the shm_general_t structure.
 * @param id The shared memory ID to be set for cargoes.
 */
void shm_cargo_set_id(shm_general_t *g, int id);

/**
 * @brief Sets the shared memory ID for the offer information structure.
 * @param g  Pointer to the shm_general_t structure.
 * @param id The shared memory ID to be set for offers.
 */
void shm_offer_set_id(shm_general_t *g, int id);

/**
 * @brief Sets the shared memory ID for the demand information structure.
 * @param g  Pointer to the shm_general_t structure.
 * @param id The shared memory ID to be set for demands.
 */
void shm_demand_set_id(shm_general_t *g, int id);

/* SHM id getters */

/**
 * @brief Gets the shared memory ID for the general information structure.
 * @param g Pointer to the shm_general_t structure.
 * @return The shared memory ID for the general information structure.
 */
int shm_general_get_id(shm_general_t *g);

/**
 * @brief Gets the shared memory ID for the ship information structure.
 * @param g Pointer to the shm_general_t structure.
 * @return The shared memory ID for the ship information structure.
 */
int shm_ship_get_id(shm_general_t *g);

/**
 * @brief Gets the shared memory ID for the port information structure.
 * @param g Pointer to the shm_general_t structure.
 * @return The shared memory ID for the port information structure.
 */
int shm_port_get_id(shm_general_t *g);

/**
 * @brief Gets the shared memory ID for the cargo information structure.
 * @param g Pointer to the shm_general_t structure.
 * @return The shared memory ID for the cargo information structure.
 */
int shm_cargo_get_id(shm_general_t *g);

/**
 * @brief Gets the shared memory ID for the offer information structure.
 * @param g Pointer to the shm_general_t structure.
 * @return The shared memory ID for the offer information structure.
 */
int shm_offer_get_id(shm_general_t *g);

/**
 * @brief Gets the shared memory ID for the demand information structure.
 * @param g Pointer to the shm_general_t structure.
 * @return The shared memory ID for the demand information structure.
 */
int shm_demand_get_id(shm_general_t *g);

/* Semaphores id getters */

/**
 * @brief Gets the semaphore ID for the start semaphore.
 * @param g Pointer to the shm_general_t structure.
 * @return The semaphore ID for the start semaphore.
 */
int sem_start_get_id(shm_general_t *g);

/**
 * @brief Gets the semaphore ID for the port initialization semaphore.
 * @param g Pointer to the shm_general_t structure.
 * @return The semaphore ID for the port initialization semaphore.
 */
int sem_port_init_get_id(shm_general_t *g);

/**
 * @brief Gets the semaphore ID for the dump semaphore.
 * @param g Pointer to the shm_general_t structure.
 * @return The semaphore ID for the dump semaphore.
 */
int sem_cargo_get_id(shm_general_t *g);

/* Message queues id getters */

/**
 * @brief Gets the message queue ID for incoming messages.
 * @param g Pointer to the shm_general_t structure.
 * @return The message queue ID for incoming messages.
 */
int msg_in_get_id(shm_general_t *g);

/**
 * @brief Gets the message queue ID for outgoing messages.
 *
 * @param g Pointer to the shm_general_t structure.
 * @return The message queue ID for outgoing messages.
 */
int msg_out_get_id(shm_general_t *g);

/* Day getter and setter */

/**
 * @brief Gets the current day from the general information structure.
 * @param g Pointer to the shm_general_t structure.
 * @return The current day value.
 */
int get_current_day(shm_general_t *g);

/**
 * @brief Increases the current day counter in the shared memory structure.
 * @param c Pointer to the general shared memory structure.
 */
void increase_day(shm_general_t *g);

/* Getters for simulation constants passed by file. */

double get_lato(shm_general_t *g);
int get_days(shm_general_t *g);
int get_navi(shm_general_t *g);
int get_speed(shm_general_t *g);
int get_capacity(shm_general_t *g);
int get_porti(shm_general_t *g);
int get_banchine(shm_general_t *g);
int get_fill(shm_general_t *g);
int get_load_speed(shm_general_t *g);
int get_merci(shm_general_t *g);
int get_size(shm_general_t *g);
int get_min_vita(shm_general_t *g);
int get_max_vita(shm_general_t *g);
int get_storm_duration(shm_general_t *g);
int get_swell_duration(shm_general_t *g);
int get_maelstrom(shm_general_t *g);

#endif
