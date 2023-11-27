#ifndef OS_PROJECT_LIST_H
#define OS_PROJECT_LIST_H

struct node_msg {
	int id;
	int quantity;
	int expire;
};

typedef struct o_list o_list_t;

/**
 * @brief creates a list of cargoes of the same type.
 * @return a pointer to o_list_t
 */
/* TODO gestire come array o singole liste?? */
o_list_t *cargo_list_create(shm_general_t *c);

/**
 * @brief adds a node to the list of cargoes ordered by expire.
 * @param list the list of cargoes
 * @param quantity quantity of the goods
 * @param expire expiration date of the goods
 */
void cargo_list_add(o_list_t *list, int type, int quantity, int expire);

/**
 * @brief removes a node with a specific expiration date from the list.
 * @param list the list of cargoes
 * @param expire_day expiration date
 * @return the quantity of goods removed
 */
int *cargo_list_remove_expired(o_list_t *list, shm_general_t *c);

/**
 * @brief pops the expires of needed quantity for element id
 * @param list
 * @param quantity
 * @return
 */
o_list_t *cargo_list_pop_needed(o_list_t *list, shm_general_t *c, int id,
				int quantity);

/**
 * @brief get number of elements of id
 * @param list
 * @param quantity
 * @param id
 * @return quantity of the element id; -1 if error
 */
int cargo_list_get_quantity(o_list_t *list, int id);

/**
 * @brief get first expire of the element id
 * @param list
 * @param quantity
 * @param id
 * @return expire of the element; -1 if there are no elements
 */
int cargo_list_get_first_expire(o_list_t *list, shm_general_t *c, int id);

/**
 * @brief Merges two cargo lists by adding the nodes from the merge list to the source list.
 * This function combines two cargo lists.
 * The source list is updated with the merged nodes.
 * @param src Pointer to the source cargo list.
 * @param merge Pointer to the cargo list whose nodes will be merged into the source list.
 * @param c Pointer to the shared memory structure containing general information.
 */
void cargo_list_merge(o_list_t *src, o_list_t *merge, shm_general_t *c);

/**
 * @brief deletes the list of cargoes.
 * @param list the list of cargoes
 */
void cargo_list_delete(o_list_t *list, shm_general_t *c);

/**
 * @brief Extracts the first order from a cargo list and returns it as a node_msg structure.
 * This function removes the first order from the cargo list, creates a node_msg structure containing
 * information about the extracted order, and returns a pointer to the created structure.
 * @param list Pointer to the cargo list from which to extract the order.
 * @param c Pointer to the shared memory structure containing general information.
 * @return A pointer to the node_msg structure representing the extracted order, or NULL if the list is empty.
 */
struct node_msg *cargo_list_pop_order(o_list_t *list, shm_general_t *c);

/**
 * @brief Prints all elements in a cargo list, displaying type, quantity, and expiration information.
 * @param list Pointer to the cargo list to be printed.
 * @param c Pointer to the shared memory structure containing general information.
 */
void cargo_list_print_all(o_list_t *list, shm_general_t *c);

#endif
