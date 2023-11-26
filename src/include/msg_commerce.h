#ifndef OS_PROJECT_MSG_COMMERCE_H
#define OS_PROJECT_MSG_COMMERCE_H

#include <sys/msg.h>

struct commerce_msg {
	long receiver;
	long sender;
	int cargo_id;
	int quantity;
	int expiry_date;
	int status;
};

int msg_commerce_in_port_init();
int msg_commerce_out_port_init();

int msg_commerce_in_port_get_id();
int msg_commerce_out_port_get_id();

#endif
