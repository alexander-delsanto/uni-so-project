#ifndef OS_PROJECT_CARGO_LIST_H
#define OS_PROJECT_CARGO_LIST_H

/**
 * @brief Cargo list structure.
 */
typedef struct o_list o_list_t;

/**
 * @brief Creates a new cargo list.
 * @return A pointer to the newly created cargo list.
 */
o_list_t *cargo_list_create(void);

/**
 * @brief Adds a cargo item to the cargo list.
 * @param list The cargo list.
 * @param quantity Quantity of the cargo item.
 * @param expire Expiration date of the cargo item.
 */
void cargo_list_add(o_list_t *list, int quantity, int expire);

/**
 * @brief Removes cargo items with a specific expire from the list.
 * @param list The cargo list.
 * @param day The expiration date to check against.
 * @return The amount of removed cargo items.
 */
int cargo_list_remove_expired(o_list_t *list, int day);

/**
 * @brief Pops cargo items from the cargo list based on the needed quantity.
 *
 * 	Creates a new list to store the popped elements and returns it.
 *
 * @param list The cargo list.
 * @param quantity The needed quantity.
 * @return the list with popped elements.
 */
o_list_t *cargo_list_pop_needed(o_list_t *list, int quantity);

/**
 * @brief Deletes the list and frees up memory.
 * @param list The cargo list to be deleted.
 */
void cargo_list_delete(o_list_t *list);

/**
 * @brief Prints all cargo items in the cargo list.
 * @param list The cargo list.
 */
void cargo_list_print_all(o_list_t *list);

/**
 * @brief Pops the first element from the cargo list.
 * 	Stores its quantity and expiration date in pointers passed by param.
 * @param list The cargo list.
 * @param quantity Pointer to store the popped quantity.
 * @param expire_day Pointer to store the popped expiration date.
 */
void cargo_list_pop(o_list_t *list, int *quantity, int *expire_day);

/**
 * @brief Gets the total quantity of non-expired cargo items in the cargo list by a specific day.
 * @param list The cargo list.
 * @param expire_day The expiration date to check against.
 * @return The total quantity of non-expired cargo items.
 */
int cargo_list_get_not_expired_by_day(o_list_t *list, int expire_day);

/**
 * @brief Gets the total quantity of all cargo items in the cargo list.
 * @param list The cargo list.
 * @return The total quantity of all cargo items.
 */
int cargo_list_get_quantity(o_list_t *list);

#endif
