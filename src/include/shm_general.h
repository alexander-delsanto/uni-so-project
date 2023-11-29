#ifndef OS_PROJECT_SHM_GENERAL_H
#define OS_PROJECT_SHM_GENERAL_H

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
void shm_ship_set_id(shm_general_t *g, int id);
void shm_port_set_id(shm_general_t *g, int id);
void shm_cargo_set_id(shm_general_t *g, int id);
void shm_offer_set_id(shm_general_t *g, int id);
void shm_demand_set_id(shm_general_t *g, int id);

/* message setters */
void set_msg_in_id(shm_general_t *g, int id);
void set_msg_out_id(shm_general_t *g, int id);

/* SHM id getters */
int shm_general_get_id(shm_general_t *g);
int shm_ship_get_id(shm_general_t *g);
int shm_port_get_id(shm_general_t *g);
int shm_cargo_get_id(shm_general_t *g);
int shm_offer_get_id(shm_general_t *g);
int shm_demand_get_id(shm_general_t *g);

/* Semaphores id getters */
int sem_start_get_id(shm_general_t *g);
int sem_port_init_get_id(shm_general_t *g);
int sem_dump_get_id(shm_general_t *g);

/* Message queues id getters */
int msg_in_get_id(shm_general_t *g);
int msg_out_get_id(shm_general_t *g);

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

int get_current_day(shm_general_t *g);

/**
 * @brief Increases the current day counter in the shared memory structure.
 * @param c Pointer to the general shared memory structure.
 */
void increase_day(shm_general_t *g);

#endif
