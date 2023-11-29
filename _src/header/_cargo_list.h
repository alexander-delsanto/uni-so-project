/*
#ifndef OS_PROJECT__CARGO_LIST_H
#define OS_PROJECT_LIST_H

#include "../../src/include/shm_cargo.h"
#include "../../src/include/shm_ship.h"

struct node_msg {
	int id;
	int quantity;
	int expire;
};

typedef struct o_list o_list_t;

*/
/**
 * @brief creates a list of cargoes of the same type.
 * @param g pointer to general SHM
 * @return a pointer to o_list_t
 *//*

*/
/* TODO gestire come array o singole liste?? *//*

o_list_t *cargo_list_create(shm_general_t *g);

*/
/**
 * @brief adds a node to the list of cargoes ordered by expire.
 * @param list the list of cargoes
 * @param quantity quantity of the goods
 * @param expire expiration date of the goods
 *//*

void cargo_list_add(o_list_t *list, int type, int quantity, int expire);

*/
/**
 * @brief removes a node with a specific expiration date from the list of a port.
 * @param list the list of cargoes
 * @param g pointer to general SHM
 * @return the quantity of goods removed
 *//*

int cargo_list_port_remove_expired(o_list_t *list, shm_general_t *g, shm_cargo_t *c);

*/
/**
 * @brief removes a node with a specific expiration date from the list of a ship.
 * @param list the list of cargoes
 * @param g pointer to general SHM
 * @param s pointer to ship SHM
 * @param ship_id identifier of the ship
 * @param c pointer to cargo SHM
 * @return the quantity of goods removed
 *//*

int cargo_list_ship_remove_expired(o_list_t *list, shm_general_t *g, shm_ship_t *s, int ship_id, shm_cargo_t *c);

*/
/**
 * @brief pops the expires of needed quantity for element id
 * @param list
 * @param g
 * @param id
 * @param quantity
 * @return TODO
 *//*

o_list_t *cargo_list_pop_needed(o_list_t *list, shm_general_t *g, int id,
				int quantity);

*/
/**
 * @brief get number of elements of id
 * @param list
 * @param id
 * @return quantity of the element id; -1 if error
 *//*

int cargo_list_get_quantity_by_id(o_list_t *list, int id);

*/
/**
 * @brief get number of elements of the list
 * @param list
 * @param g
 * @return quantity of the list; -1 if error
 *//*

int cargo_list_get_quantity(o_list_t *list, shm_general_t *g);

*/
/**
 * @brief get first expire of the element id
 * @param list
 * @param g
 * @param id
 * @return expire of the element; -1 if there are no elements
 *//*

int cargo_list_get_first_expire(o_list_t *list, shm_general_t *g, int id);

*/
/**
 * @brief Merges two cargo lists by adding the nodes from the merge list to the source list.
 * This function combines two cargo lists.
 * The source list is updated with the merged nodes.
 * @param src Pointer to the source cargo list.
 * @param merge Pointer to the cargo list whose nodes will be merged into the source list.
 * @param g Pointer to the shared memory structure containing general information.
 *//*

void cargo_list_merge(o_list_t *src, o_list_t *merge, shm_general_t *g);

*/
/**
 * @brief deletes the list of cargoes.
 * @param g pointer to general SHM
 * @param list the list of cargoes
 *//*

void cargo_list_delete(o_list_t *list, shm_general_t *g);

*/
/**
 * @brief Extracts the first order from a cargo list and returns it as a node_msg structure.
 * This function removes the first order from the cargo list, creates a node_msg structure containing
 * information about the extracted order, and returns a pointer to the created structure.
 * @param list Pointer to the cargo list from which to extract the order.
 * @param g Pointer to the shared memory structure containing general information.
 * @return A pointer to the node_msg structure representing the extracted order, or NULL if the list is empty.
 *//*

struct node_msg *cargo_list_pop_order(o_list_t *list, shm_general_t *g);

*/
/**
 * @brief Returns the amount of cargo of cargo_type not expired by a certain day.
 * @param list Pointer to the source cargo list.
 * @param cargo_type The type of the cargo.
 * @param day The day by which the cargo is not expired.
 * @return The amount of cargo not expired by day.
 *//*

int get_not_expired_by_day(o_list_t *list, int cargo_type, int day);

*/
/**
 * @brief Prints all elements in a cargo list, displaying type, quantity, and expiration information.
 * @param list Pointer to the cargo list to be printed.
 * @param g Pointer to the shared memory structure containing general information.
 *//*

void cargo_list_print_all(o_list_t *list, shm_general_t *g);

void cargo_list_pop(o_list_t *list, int *quantity, int *expiration_date);

void cargo_list_print_single(o_list_t *list);

#endif
*/
