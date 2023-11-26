#ifndef OS_PROJECT_MSG_COMMERCE_H
#define OS_PROJECT_MSG_COMMERCE_H

#include <sys/msg.h>

#include "types.h"

typedef struct commerce_msg commerce_msg_t;

int msg_commerce_in_port_init(void);
int msg_commerce_out_port_init(void);

int msg_commerce_in_port_get_id(void);
int msg_commerce_out_port_get_id(void);

commerce_msg_t msg_commerce_create(long sender_id, long receiver_id, int cargo_id, int quantity, int expiry_date, int status);
void msg_commerce_send(int queue_id, commerce_msg_t *msg);
bool_t msg_commerce_receive(int queue_id, int type, long *sender_id, int *cargo_id, int *quantity, int *expiry_date, int *status, bool_t restarting);

#endif
