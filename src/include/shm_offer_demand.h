#ifndef OS_PROJECT_OFFER_H
#define OS_PROJECT_OFFER_H

#include <stdlib.h>

#include "shm_general.h"
#include "shm_cargo.h"
#include "cargo_list.h"
#include "types.h"

/**
 * @brief Structure representing shared memory for offer data.
 */
typedef struct shm_offer shm_offer_t;

/**
 * @brief Structure representing shared memory for demand data.
 */
typedef struct shm_demand shm_demand_t;

/**
 * @brief Initializes and attaches shared memory for offer data associated with ports.
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
 * @param o Pointer to the array of offers in SHM.
 * @param g Pointer to shared memory general information.
 * @param id Port ID.
 * @param type Cargo type.
 * @param quantity quantity to add.
 */
void shm_offer_add_quantity(shm_offer_t *o, shm_general_t *g, int id, int type,
		   int quantity);

/**
 * @brief Removes quantity from the shared memory for offers.
 * @param o Pointer to the array of offers in SHM.
 * @param g Pointer to shared memory general information.
 * @param id Port ID.
 * @param type Cargo type.
 * @param quantity quantity to remove.
 */
void shm_offer_remove_quantity(shm_offer_t *o, shm_general_t *g, int id, int type,
		      int quantity);

/**
 * @brief Deletes the shared memory for offers, freeing allocated memory.
 * @param o Pointer to the shared memory offer structure to delete.
 */
void shm_offer_delete(shm_offer_t *o);

/**
 * @brief Gets the quantity of offers for a specific cargo type at a port.
 * @param g Pointer to shared memory general information.
 * @param o Pointer to shared memory for offers.
 * @param port_id Port ID.
 * @param cargo_id Cargo ID.
 * @return Quantity of offers for the specified cargo type at the port.
 */
int shm_offer_get_quantity(shm_general_t *g, shm_offer_t *o, int port_id, int cargo_id);

/**
 * @brief Gets the total quantity of offers for all cargo types at a port.
 * @param g Pointer to shared memory general information.
 * @param o Pointer to shared memory for offers.
 * @param port_id Port ID.
 * @return Total quantity of offers for all cargo types at the port.
 */
int shm_offer_get_tot_quantity(shm_general_t *g, shm_offer_t *o, int port_id);

/**
 * @brief Combines data from two arrays of shm_offer_t structures.
 * @param src Source array of shared memory for offers.
 * @param merge Array (local) to merge.
 * @param g Pointer to shared memory general information.
 * @param id Port ID.
 */
void shm_offer_merge(shm_offer_t *src, shm_offer_t *merge, shm_general_t *g,
		     int id);

/**
 * @brief Gets a new array of shared memory for offers representing an order.
 * @param @param o Pointer to shared memory for offers.
 * @param g Pointer to shared memory general information.
 * @param id Port ID.
 * @param capacity Order capacity.
 * @return A pointer to a new shared memory offer containing the retrieved goods.
 */
shm_offer_t *shm_offer_get_order(shm_offer_t *o, shm_general_t *g, int id,
				 int capacity);
/**
 * @brief Initializes and attaches shared memory for demand data associated with ports.
 * @param g Pointer to general SHM
 * @return Pointer to the attached demand data structure or NULL on failure.
 */
shm_demand_t *shm_demand_init(shm_general_t *g);

/**
 * @brief Sets the demand for a specific cargo type at a port.
 * @param d Pointer to the array of demands in SHM.
 * @param g Pointer to general SHM.
 * @param id Port ID.
 * @param type Cargo type.
 * @param quantity Demand quantity to set.
 */
void shm_demand_set(shm_demand_t *d, shm_general_t *g, int id, int type,
		    int quantity);

/**
 * @brief Gets the quantity of demand for a specific cargo type at a port.
 * @param g Pointer to shared memory general information.
 * @param d Pointer to shared memory for demand.
 * @param port_id Port ID.
 * @param cargo_id Cargo ID.
 * @return Quantity of demand for the specified cargo type at the port.
 */
int shm_demand_get_quantity(shm_general_t *g, shm_demand_t *d, int port_id, int cargo_id);

/**
 * @brief Removes quantity from the shared memory for demand.
 * @param d Pointer to the array of demands in SHM.
 * @param g Pointer to general SHM.
 * @param id Port ID.
 * @param type Cargo type.
 * @param quantity Demand quantity to remove.
 */
void shm_demand_remove_quantity(shm_demand_t *d, shm_general_t *g, int id, int type,
		       int quantity);

/**
 * @brief Deletes the shared memory for both offers and demands.
 * @param g Pointer to general SHM.
 */
void shm_offer_demand_delete(shm_general_t *g);

/**
 * @brief Generates random offers and demands.
 * @param o Pointer to shared memory for offers.
 * @param d Pointer to shared memory for demands.
 * @param l Array of cargo lists.
 * @param port_id Port ID.
 * @param c Pointer to cargo SHM
 * @param g Pointer to general SHM
 */
void shm_offer_demand_generate(shm_offer_t *o, shm_demand_t *d, o_list_t **l,
			       int port_id, shm_cargo_t *c, shm_general_t *g);



#endif
