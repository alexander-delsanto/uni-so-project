#ifndef OS_PROJECT_MSG_COMMERCE_H
#define OS_PROJECT_MSG_COMMERCE_H

#include <sys/msg.h>

#include "types.h"

struct commerce_msg {
	long receiver;
	long sender;
	int cargo_id;
	int quantity;
	int expiry_date;
	int status;
};

int msg_commerce_in_port_init(void);
int msg_commerce_out_port_init(void);

int msg_commerce_in_port_get_id(void);
int msg_commerce_out_port_get_id(void);

struct commerce_msg msg_commerce_create(long receiver_id, long sender_id, int cargo_id, int quantity, int expiry_date, int status);
void msg_commerce_send(int queue_id, struct commerce_msg *msg);
bool_t msg_commerce_receive(int queue_id, int type, int *sender_id, int *cargo_id, int *quantity, int *expiry_date, int *status, bool_t restarting);

#endif
