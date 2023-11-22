#ifndef OS_PROJECT_LIST_H
#define OS_PROJECT_LIST_H

typedef struct o_list o_list_t;

o_list_t *OrderedList(void);

void add(o_list_t *list, int quantity, int expire);
int remove_expired(o_list_t *list, int expire_day);
o_list_t *pop_needed(o_list_t *list, int quantity);
void delete_list(o_list_t *list);
void print_all(o_list_t *list);

#endif
