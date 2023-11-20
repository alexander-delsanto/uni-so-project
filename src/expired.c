#include "include/shm_config.h"
#include "include/expired.h"

struct expired_data {
	int qty;
	int expire_date;
	struct expired_data *next; /* sarà una lista ordinata */
};

struct expired {
	struct expired_data *head;
};

expired_t *expired_init(shm_config_t *c)
{
	expired_t *exp;
	int i;

	exp = malloc(sizeof(expired_t) * get_merci(c));
	if (exp == NULL) {
		return NULL;
	}
	for (i = 0; i < get_merci(c); i++) {
		exp[i].head = NULL;
	}

	return exp;
}
