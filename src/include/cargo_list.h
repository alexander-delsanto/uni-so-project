#ifndef OS_PROJECT_LIST_H
#define OS_PROJECT_LIST_H

typedef struct o_list o_list_t;

o_list_t *cargo_list_create(void);

void cargo_list_add(o_list_t *list, int quantity, int expire);
int cargo_list_remove_expired(o_list_t *list, int expire_day);
o_list_t *cargo_list_pop_needed(o_list_t *list, int quantity);
void cargo_list_delete(o_list_t *list);
void cargo_list_print_all(o_list_t *list);

#endif
