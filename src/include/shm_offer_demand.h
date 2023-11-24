#ifndef OS_PROJECT_OFFER_H
#define OS_PROJECT_OFFER_H

#include <stdlib.h>
#include "shm_general.h"
#include "cargo_list.h"
#include "types.h"

typedef struct shm_offer shm_offer_t;
typedef struct shm_demand shm_demand_t;

/**
 * @brief Initializes and attaches shared memory for offer data.
 * @param c pointer to general SHM
 * @return Pointer to the attached offer data structure or NULL on failure.
 */
shm_offer_t *offer_shm_init(shm_general_t *c);

/**
 * @brief Initializes and attaches shared memory for demand data.
 * @param c pointer to general SHM
 * @return Pointer to the attached demand data structure or NULL on failure.
 */
shm_demand_t *demand_shm_init(shm_general_t *c);

/**
 * @brief TODO: generates random offers and demands
 * @param o pointer to the array of offers
 * @param d pointer to the array of demands
 * @param l pointer to the array of expires
 * @param id the id of the entity offering or demanding
 * @param c pointer to general SHM
 */
void offer_demand_shm_generate(shm_offer_t *o, shm_demand_t *d, o_list_t *l,
			       int id, shm_general_t *c);

/**
 * @brief Adds a specified quantity to the offer data in shared memory.
 * @param o pointer to the array of offers
 * @param id the id of the entity for which the offer data is updated
 * @param type the type of the offer data to be updated
 * @param quantity quantity to be added to the offer data
 */
void offer_shm_add(shm_offer_t *o, int id, int type, int quantity);
/**
 * @brief Removes a specified quantity from the offer data in shared memory.
 * @param o pointer to the array of offers in shared memory
 * @param id identifier of the entity for which the offer data is updated
 * @param type type of the offer data to be updated
 * @param quantity quantity to be subtracted from the offer data
 */
void offer_shm_remove(shm_offer_t *o, int id, int type, int quantity);

/**
 * @brief Sets the demand data in shared memory for a specific entity and type.
 * @param o pointer to the array of demands
 * @param id the id of the entity for which the demand data is updated
 * @param type the type of the demand data to be updated
 * @param quantity quantity to be added to the demand data
 */
void demand_shm_set(shm_demand_t *d, int id, int type, int quantity);
/**
 * @brief Removes a specified quantity from the demand data in shared memory.
 * @param d Pointer to the array of demands in shared memory.
 * @param id Identifier of the entity for which the demand data is updated.
 * @param type Type of the demand data to be updated.
 * @param quantity Quantity to be subtracted from the demand data.
 */
void demand_shm_remove(shm_demand_t *d, int id, int type, int quantity);

#endif
