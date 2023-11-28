#ifndef OS_PROJECT_OFFER_H
#define OS_PROJECT_OFFER_H

#include <stdlib.h>
#include "shm_general.h"
#include "shm_cargo.h"
#include "cargo_list.h"
#include "types.h"

typedef struct shm_offer shm_offer_t;
typedef struct shm_demand shm_demand_t;

/**
 * @brief Initializes and attaches shared memory for offer data in ports.
 * @param g pointer to general SHM
 * @return Pointer to the attached offer data structure or NULL on failure.
 */
shm_offer_t *shm_offer_ports_init(shm_general_t *g);

/**
 * @brief Attaches shared memory for offer data in ports.
 * @param g pointer to general shm
 * @return Pointer to the attached offer data structure.
 */
shm_offer_t *shm_offer_ports_attach(shm_general_t *g);

/**
 * @brief Adds a specified quantity to the offer data in shared memory.
 * @param o pointer to the array of offers
 * @param id the id of the entity for which the offer data is updated
 * @param type the type of the offer data to be updated
 * @param quantity quantity to be added to the offer data
 */
void offer_shm_set(shm_offer_t *o, shm_general_t *g, int id, int type,
		   int quantity);

/**
 * @brief Removes a specified quantity from the offer data in shared memory.
 * @param o pointer to the array of offers in shared memory
 * @param id identifier of the entity for which the offer data is updated
 * @param type type of the offer data to be updated
 * @param quantity quantity to be subtracted from the offer data
 */
void offer_shm_remove(shm_offer_t *o, shm_general_t *g, int id, int type,
		      int quantity);

/**
 * @brief Delete a shared memory offer structure, freeing allocated memory.
 * @param o Pointer to the shared memory offer structure to be deleted.
 */
void offer_shm_delete(shm_offer_t *o);

/**
 * @brief Removes a specified quantity from the demand data in shared memory.
 * @param src Pointer to the array of source offers in shared memory.
 * @param merge Pointer to local array of offers to be merged.
 * @param c pointer to general SHM
 * @param id Identifier of the entity for which the demand data is updated.
 */
void offer_shm_merge(shm_offer_t *src, shm_offer_t *merge, shm_general_t *c,
		     int id);

/**
 * @brief Gets a portion of goods from a specific offer in shared memory.
 * @param o Pointer to the array of shared memory offers.
 * @param g Pointer to the shared memory structure containing general information.
 * @param id The index of the offer to retrieve goods from.
 * @param capacity The maximum capacity of goods to retrieve.
 *
 * @return A pointer to a new shared memory offer containing the retrieved goods.
 */
shm_offer_t *offer_shm_get_order(shm_offer_t *o, shm_general_t *g, int id,
				 int capacity);
/**
 * @brief Gets expired goods from a source offer list.
 * @param src Pointer to the source offer list.
 * @param o Pointer to the array of shared memory offers.
 * @param g Pointer to the shared memory structure containing general information.
 * @return A pointer to a new offer list containing the retrieved expired goods.
 */
o_list_t *offer_shm_get_order_expires(o_list_t *src, shm_offer_t *o,
				      shm_general_t *g);

/**
 * @brief Initializes and attaches shared memory for demand data.
 * @param g pointer to general SHM
 * @return Pointer to the attached demand data structure or NULL on failure.
 */
shm_demand_t *demand_shm_init(shm_general_t *g);

/**
 * @brief Sets the demand data in shared memory for a specific entity and type.
 * @param o pointer to the array of demands
 * @param g pointer to general SHM
 * @param id the id of the entity for which the demand data is updated
 * @param type the type of the demand data to be updated
 * @param quantity quantity to be added to the demand data
 */
void demand_shm_add(shm_demand_t *d, shm_general_t *g, int id, int type,
		    int quantity);

/**
 * @brief Removes a specified quantity from the demand data in shared memory.
 * @param d Pointer to the array of demands in shared memory.
 * @param g pointer to general SHM
 * @param id Identifier of the entity for which the demand data is updated.
 * @param type Type of the demand data to be updated.
 * @param quantity Quantity to be subtracted from the demand data.
 */
void demand_shm_remove(shm_demand_t *d, shm_general_t *g, int id, int type,
		       int quantity);

/**
 * @brief Return the quantity of requested item
 * @param d Pointer to the array of demands in shared memory.
 * @param id Identifier of the port for which the demand data is requested.
 * @param type Type of the demand data to be requested.
 */
/*int demand_shm_get(shm_demand_t *d, int id, int type);*/


/**
 * Delete offer and demand shared memory
 * @param g pointer to general SHM
 */
void offer_demand_shm_delete(shm_general_t *g);

/**
 * @brief Generates random offers and demands
 * @param o pointer to the array of offers
 * @param d pointer to the array of demands
 * @param l pointer to the array of expires
 * @param id the id of the entity offering or demanding
 * @param c pointer to cargo SHM
 * @param g pointer to general SHM
 */
void offer_demand_shm_generate(shm_offer_t *o, shm_demand_t *d, o_list_t *l,
			       int id, shm_cargo_t *c, shm_general_t *g);

int shm_offer_get_quantity(shm_general_t *g, shm_offer_t *o, int port_id, int cargo_id);
int shm_demand_get_quantity(shm_general_t *g, shm_demand_t *d, int port_id, int cargo_id);

#endif
