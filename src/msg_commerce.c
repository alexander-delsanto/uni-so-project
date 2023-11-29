#define _GNU_SOURCE

#include <stdio.h>

#include "include/msg_commerce.h"
#include "include/const.h"

#define MSG_SIZE (sizeof(struct commerce_msg) - sizeof(long))
#define MSG_TYPE(type) ((type) + 1)

int msg_commerce_in_port_init(void)
{
	int id;
	if ((id = msgget(MSG_IN_PORT_KEY, 0660 | IPC_CREAT | IPC_EXCL)) < 0)
		dprintf(2, "msg_commerce.c - msg_commerce_in_port_init: Failed to create message queue.\n");
	return id;
}

int msg_commerce_out_port_init(void)
{
	int id;
	if ((id = msgget(MSG_OUT_PORT_KEY, 0660 | IPC_CREAT | IPC_EXCL)) < 0)
		dprintf(2, "msg_commerce.c - msg_commerce_in_port_init: Failed to create message queue.\n");
	return id;
}

int msg_commerce_in_port_get_id(void)
{
	int id;
	if ((id = msgget(MSG_IN_PORT_KEY, 0)) < 0)
		dprintf(2, "msg_commerce.c - msg_commerce_in_port_get_id: Failed to get message queue id.\n");

	return id;
}

int msg_commerce_out_port_get_id(void)
{
	int id;
	if ((id = msgget(MSG_OUT_PORT_KEY, 0)) < 0)
		dprintf(2, "msg_commerce.c - msg_commerce_out_port_get_id: Failed to get message queue id.\n");
	return id;
}

struct commerce_msg msg_commerce_create(long receiver_id, long sender_id,
					int cargo_id, int quantity,
					int expiry_date, int status)
{
	struct commerce_msg res;
	res.receiver = MSG_TYPE(receiver_id);
	res.sender = sender_id;
	res.cargo_id = cargo_id;
	res.quantity = quantity;
	res.expiry_date = expiry_date;
	res.status = status;
	return res;
}

void msg_commerce_send(int queue_id, struct commerce_msg *msg)
{
	int ret;
	do {
		ret = msgsnd(queue_id, msg, MSG_SIZE, 0);
	} while (ret < 0);
}

bool_t msg_commerce_receive(int queue_id, int type, int *sender_id,
			    int *cargo_id, int *quantity, int *expiry_date,
			    int *status, bool_t restarting)
{
	ssize_t ret;
	struct commerce_msg msg;
	do {
		ret = msgrcv(queue_id, &msg, MSG_SIZE, MSG_TYPE(type), 0);
		if (!restarting && ret < 0)
			return FALSE;
	} while(ret < 0);

	if (sender_id != NULL) *sender_id = (int) msg.sender;
	if (cargo_id != NULL) *cargo_id = msg.cargo_id;
	if (quantity != NULL) *quantity = msg.quantity;
	if (expiry_date != NULL) *expiry_date = msg.expiry_date;
	if (status != NULL) *status = msg.status;

	return TRUE;
}
