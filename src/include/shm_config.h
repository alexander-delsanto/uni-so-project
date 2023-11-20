#ifndef OS_PROJECT_SHM_CONFIG_H
#define OS_PROJECT_SHM_CONFIG_H

typedef struct shm_config shm_config_t;

shm_config_t *read_from_path(char *path);
shm_config_t *config_shm_attach(void);
void config_shm_detach(shm_config_t *c);
void config_shm_delete(int id);

void set_port_shm_id(shm_config_t *c, int id);
void set_ship_shm_id(shm_config_t *c, int id);
int get_port_shm_id(shm_config_t *c);
int get_ship_shm_id(shm_config_t *c);
int get_config_shm_id(shm_config_t *c);

double get_lato(shm_config_t *c);
int get_days(shm_config_t *c);
int get_navi(shm_config_t *c);
int get_speed(shm_config_t *c);
int get_capacity(shm_config_t *c);
int get_porti(shm_config_t *c);
int get_banchine(shm_config_t *c);
int get_fill(shm_config_t *c);
int get_load_speed(shm_config_t *c);
int get_merci(shm_config_t *c);
int get_size(shm_config_t *c);
int get_min_vita(shm_config_t *c);
int get_max_vita(shm_config_t *c);
int get_storm_duration(shm_config_t *c);
int get_swell_duration(shm_config_t *c);
int get_maelstrom(shm_config_t *c);

int get_current_day(shm_config_t *c);
void increase_day(shm_config_t *c);

#endif
