#ifndef OS_PROJECT_SHM_GENERAL_H
#define OS_PROJECT_SHM_GENERAL_H

typedef struct shm_general shm_general_t;

shm_general_t *read_from_path(char *path);
shm_general_t *general_shm_attach(void);
void general_shm_detach(shm_general_t *c);
void general_shm_delete(int id);

void set_port_shm_id(shm_general_t *c, int id);
void set_ship_shm_id(shm_general_t *c, int id);
int get_port_shm_id(shm_general_t *c);
int get_ship_shm_id(shm_general_t *c);
int get_general_shm_id(shm_general_t *c);

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
void increase_day(shm_general_t *c);

#endif
