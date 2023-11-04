#ifndef _PROJECT_IPC_UTILS_H
#define _PROJECT_IPC_UTILS_H

#include "shared_memory.h"
#include "../../lib/shm.h"
#include "../../lib/semaphore.h"

/**
 * @brief Initializes and attaches general shared memory structure with shared memory constants.
 *
 * @param data a pointer to a struct data_general.
 */
void initialize_general_shm(struct data_general *data);

/**
 * @brief Initializes and attaches ship, port and cargo shared memory structures.
 */
void initialize_shm();

/* Getters */
int get_general_shm_id();
int get_ship_shm_id();
int get_cargo_shm_id();
pid_t get_ship_pid(int ship_id);
struct coordinates get_ship_coords(int ship_id);
double get_constant(int const_num);

/* Setters */
void set_ship_pid(int ship_id, pid_t ship_pid);
void set_ship_coords(int ship_id, struct coordinates coords);
void delete_all_shm();

/*
 * Macros used to get simulation constants from shared memory.
 */
#define SO_LATO ((double)get_constant(0))
#define SO_DAYS ((int)get_constant(1))
#define SO_NAVI ((int)get_constant(2))
#define SO_SPEED ((int)get_constant(3))
#define SO_CAPACITY ((int)get_constant(4))
#define SO_PORTI ((int)get_constant(5))
#define SO_BANCHINE ((int)get_constant(6))
#define SO_FILL ((int)get_constant(7))
#define SO_LOADSPEED ((int)get_constant(8))
#define SO_MERCI ((int)get_constant(9))
#define SO_SIZE ((int)get_constant(10))
#define SO_MIN_VITA ((int)get_constant(11))
#define SO_MAX_VITA ((int)get_constant(12))
#define SO_STORM_DURATION ((int)get_constant(13))
#define SO_SWELL_DURATION ((int)get_constant(14))
#define SO_MAELSTROM ((int)get_constant(15))

#endif

