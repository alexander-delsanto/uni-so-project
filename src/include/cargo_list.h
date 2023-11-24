#ifndef OS_PROJECT_LIST_H
#define OS_PROJECT_LIST_H

typedef struct o_list o_list_t;

/**
 * @brief creates a list of cargoes of the same type.
 * @return a pointer to o_list_t
 */
o_list_t *cargo_list_create(void);

/**
 * @brief adds a node to the list of cargoes ordered by expire.
 * @param list the list of cargoes
 * @param quantity quantity of the goods
 * @param expire expiration date of the goods
 */
void cargo_list_add(o_list_t *list, int quantity, int expire);

/**
 * @brief removes a node with a specific expiration date from the list.
 * @param list the list of cargoes
 * @param expire_day expiration date
 * @return the quantity of goods removed
 */
int cargo_list_remove_expired(o_list_t *list, int expire_day);

/**
 * TODO: non ho capito ops
 * @param list
 * @param quantity
 * @return
 */
o_list_t *cargo_list_pop_needed(o_list_t *list, int quantity);

/**
 * @brief deletes the list of cargoes.
 * @param list the list of cargoes
 */
void cargo_list_delete(o_list_t *list);

void cargo_list_print_all(o_list_t *list);

#endif
