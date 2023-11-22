#include "include/shm_general.h"
#include "include/expired.h"

struct expired_data {
	int qty;
	int expire_date;
	struct expired_data *next; /* sarà una lista ordinata */
};

struct expired {
	struct expired_data *head;
};

expired_t *expired_init(shm_general_t *c)
{
	expired_t *exp;
	int i;

	exp = calloc(get_merci(c), sizeof(expired_t));
	if (exp == NULL) {
		return NULL;
	}

	return exp;
}
