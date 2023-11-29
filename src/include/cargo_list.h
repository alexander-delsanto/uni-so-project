#ifndef OS_PROJECT_CARGO_LIST_H
#define OS_PROJECT_CARGO_LIST_H

typedef struct o_list o_list_t;

o_list_t *cargo_list_create(void);

/**
 * @brief Adds quantity on list with a specific expire.
 * @param list
 * @param quantity
 * @param expire
 */
void cargo_list_add(o_list_t *list, int quantity, int expire);

/**
 * @brief Removes cargo with a specific expire from the list.
 * @param list
 * @param expire_day
 * @return the amount of removed elements.
 */
int cargo_list_remove_expired(o_list_t *list, int expire_day);

/**
 * @brief Pops a specific quantity elements from the head of the list.
* 	Creates a new list to store the popped elements and returns it.
 * @param list
 * @param quantity
 * @return the list with popped elements.
 */
o_list_t *cargo_list_pop_needed(o_list_t *list, int quantity);

/**
 * @brief Deletes the list.
 * @param list
 */
void cargo_list_delete(o_list_t *list);

/**
 * @brief Prints all elements in the given linked list.
 * @param list
 */
void cargo_list_print_all(o_list_t *list);

/**
 * @brief Pops the first element from the linked list and return its quantity and expiration date.
 * @param list
 * @param quantity
 * @param expire_day
 */
void cargo_list_pop(o_list_t *list, int *quantity, int *expire_day);

/**
 * @brief Calculates the quantity of unexpired elements in a specific expire day.
 * @param list
 * @param expire_day
 * @return the amount calculated.
 */
int cargo_list_get_not_expired_by_day(o_list_t *list, int expire_day);

/**
 * @brief Calculate the total quantity of items in the linked list.
 * @param list
 * @return the amount calculated.
 */
int cargo_list_get_quantity(o_list_t *list);

#endif
