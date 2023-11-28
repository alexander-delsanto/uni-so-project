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
 * @brief Attaches the process to the shared memory segment for the general structure.
 * @param g Pointer to the pointer of the general shared memory structure.
 */
void general_shm_attach(shm_general_t **g);
/*shm_general_t *general_shm_attach(void);*/
/**
 * @brief Detaches the process from the shared memory segment for the general structure.
 * @param c Pointer to the general shared memory structure.
 */
void general_shm_detach(shm_general_t *c);
/**
 * @brief Deletes the shared memory segment with the specified ID.
 * @param id Shared memory ID to be deleted.
 */
void general_shm_delete(int id);

void set_ship_shm_id(shm_general_t *c, int id);
void set_port_shm_id(shm_general_t *c, int id);
void set_cargo_shm_id(shm_general_t *g, int id);
void set_offer_shm_id(shm_general_t *c, int id);
void set_demand_shm_id(shm_general_t *c, int id);
void set_msg_in_id(shm_general_t *c, int id);
void set_msg_out_id(shm_general_t *c, int id);

int get_general_shm_id(shm_general_t *c);
int get_ship_shm_id(shm_general_t *c);
int get_port_shm_id(shm_general_t *c);
int get_cargo_shm_id(shm_general_t *c);
int get_offer_shm_id(shm_general_t *c);
int get_demand_shm_id(shm_general_t *c);
int get_msg_in_id(shm_general_t *c);
int get_msg_out_id(shm_general_t *c);

double get_lato(shm_general_t *c);
int get_days(shm_general_t *c);
int get_navi(shm_general_t *c);
int get_speed(shm_general_t *c);
int get_capacity(shm_general_t *c);
int get_porti(shm_general_t *c);
int get_banchine(shm_general_t *c);
int get_fill(shm_general_t *c);
int get_load_speed(shm_general_t *c);
int get_merci(shm_general_t *c);
int get_size(shm_general_t *c);
int get_min_vita(shm_general_t *c);
int get_max_vita(shm_general_t *c);
int get_storm_duration(shm_general_t *c);
int get_swell_duration(shm_general_t *c);
int get_maelstrom(shm_general_t *c);

int get_current_day(shm_general_t *c);

/**
 * @brief Increases the current day counter in the shared memory structure.
 * @param c Pointer to the general shared memory structure.
 */
void increase_day(shm_general_t *c);

#endif
