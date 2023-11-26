#ifndef OS_PROJECT_MSG_COMMERCE_H
#define OS_PROJECT_MSG_COMMERCE_H

#include <sys/msg.h>

#include "types.h"

/* For port to ship messages */
#define STATUS_ACCEPTED 1
#define STATUS_REFUSED 2

/* For ship to port messages */
#define STATUS_REQUEST 0
#define STATUS_DEAD (-1)
#define STATUS_MISSING 3
#define STATUS_LOAD_REQUEST 4

struct commerce_msg {
	long receiver;
	long sender;
	int cargo_id;
	int quantity;
	int expiry_date;
	int capacity;

	int status;
};

int msg_commerce_in_port_init(void);
int msg_commerce_out_port_init(void);

int msg_commerce_in_port_get_id(void);
int msg_commerce_out_port_get_id(void);

struct commerce_msg msg_commerce_create(long receiver_id, long sender_id,
					int cargo_id, int quantity,
					int expiry_date, int capacity,
					int status);
void msg_commerce_send(int queue_id, struct commerce_msg *msg);
bool_t msg_commerce_receive(int queue_id, int type, int *sender_id,
			    int *cargo_id, int *quantity, int *expiry_date,
			    int *capacity, int *status, bool_t restarting);

#endif
