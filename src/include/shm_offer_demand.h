#ifndef OS_PROJECT_OFFER_H
#define OS_PROJECT_OFFER_H

#include <stdlib.h>
#include "shm_general.h"
#include "cargo_list.h"
#include "types.h"

typedef struct shm_offer shm_offer_t;
typedef struct shm_demand shm_demand_t;

/**
 * @brief Initializes and attaches shared memory for offer data in ports.
 * @param c pointer to general SHM
 * @return Pointer to the attached offer data structure or NULL on failure.
 */
shm_offer_t *offer_shm_ports_init(shm_general_t *c);
/**
 * @brief Initializes and attaches shared memory for offer data in ships.
 * @param c pointer to general SHM
 * @return Pointer to the attached offer data structure or NULL on failure.
 */
shm_offer_t *offer_shm_ships_init(shm_general_t *c);

/**
 * @brief Adds a specified quantity to the offer data in shared memory.
 * @param o pointer to the array of offers
 * @param id the id of the entity for which the offer data is updated
 * @param type the type of the offer data to be updated
 * @param quantity quantity to be added to the offer data
 */
void offer_shm_set(shm_offer_t *o, int id, int type, int quantity);

/**
 * @brief Removes a specified quantity from the offer data in shared memory.
 * @param o pointer to the array of offers in shared memory
 * @param id identifier of the entity for which the offer data is updated
 * @param type type of the offer data to be updated
 * @param quantity quantity to be subtracted from the offer data
 */
void offer_shm_remove(shm_offer_t *o, int id, int type, int quantity);

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
 * @param c Pointer to the shared memory structure containing general information.
 * @param id The index of the offer to retrieve goods from.
 * @param capacity The maximum capacity of goods to retrieve.
 *
 * @return A pointer to a new shared memory offer containing the retrieved goods.
 */
shm_offer_t *offer_shm_get_order(shm_offer_t *o, shm_general_t *c, int id,
				 int capacity);
/**
 * @brief Gets expired goods from a source offer list.
 * @param src Pointer to the source offer list.
 * @param o Pointer to the array of shared memory offers.
 * @param c Pointer to the shared memory structure containing general information.
 * @return A pointer to a new offer list containing the retrieved expired goods.
 */
o_list_t *offer_shm_get_order_expires(o_list_t *src, shm_offer_t *o,
				      shm_general_t *c);

/**
 * @brief Initializes and attaches shared memory for demand data.
 * @param c pointer to general SHM
 * @return Pointer to the attached demand data structure or NULL on failure.
 */
shm_demand_t *demand_shm_init(shm_general_t *c);

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


/**
 * @brief Generates random offers and demands
 * @param o pointer to the array of offers
 * @param d pointer to the array of demands
 * @param l pointer to the array of expires
 * @param id the id of the entity offering or demanding
 * @param c pointer to general SHM
 */
void offer_demand_shm_generate(shm_offer_t *o, shm_demand_t *d, o_list_t *l,
			       int id, shm_general_t *c);

/**
 * @brief Performs a transaction between a ship's offer and a port's demand in shared memory.
 *
 * This function updates the quantities of goods in a ship's offer and a port's demand based on the transaction.
 * If the ship has enough goods for the demand, it deducts the demanded quantity from the ship and updates the port accordingly.
 * If the ship has insufficient goods, it deducts all available goods from the ship and updates the port's demand.
 *
 * @param o Pointer to the array of shared memory ship offers.
 * @param d Pointer to the array of shared memory port demands.
 * @param id_ship The index of the ship's offer in the shared memory.
 * @param id_port The index of the port's demand in the shared memory.
 * @param c Pointer to the shared memory structure containing general information.
 */
void offer_demand_shm_transaction(shm_offer_t *o, shm_demand_t *d, int id_ship,
				  int id_port, shm_general_t *c);

shm_offer_t *offer_shm_get_order_from_demand(shm_offer_t *o, shm_demand_t *d,
					     shm_general_t *c, int port_id,
					     int ship_id);

#endif
