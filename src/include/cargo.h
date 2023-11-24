#ifndef OS_PROJECT_CARGO_H
#define OS_PROJECT_CARGO_H

#include <stdlib.h>

#include "shm_general.h"
/*#include "expired.h"*/

typedef struct cargo cargo_t;

typedef int *cargo_exp_t;

/**
 * @brief Initializes a cargo structure for managing offers and demands in shared memory.
 * @param c Pointer to the general shared memory structure.
 * @return Pointer to the allocated cargo structure or NULL if memory allocation fails.
 */
cargo_t *cargo_initialize(shm_general_t *c);

/**
 * @brief Calculates the size of the cargo structure in bytes.
 * @param c Pointer to the general shared memory structure.
 * @return Size of the cargo structure in bytes.
 */
size_t cargo_get_size(shm_general_t *c);

/**
 * @brief Generates random offers and demands in the cargo structure.
 * @param c Pointer to the general shared memory structure.
 * @param cargo Pointer to the cargo structure to be populated.
 */
void cargo_generate(shm_general_t *c, cargo_t *cargo);

#endif
