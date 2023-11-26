#define _GNU_SOURCE

#include <stdio.h>

#include "include/msg_commerce.h"
#include "include/const.h"

struct commerce_msg {
	long receiver;
	long sender;
	int cargo_id;
	int quantity;
	int expiry_date;
	int status;
};

#define MSG_SIZE (sizeof(struct commerce_msg) - sizeof(long))

int msg_commerce_in_port_init()
{
	int id;
	if ((id = msgget(MSG_IN_PORT_KEY, 0660 | IPC_CREAT | IPC_EXCL)) < 0)
		dprintf(2, "msg_commerce.c - msg_commerce_in_port_init: Failed to create message queue.\n");
	return id;
}

int msg_commerce_out_port_init()
{
	int id;
	if ((id = msgget(MSG_OUT_PORT_KEY, 0660 | IPC_CREAT | IPC_EXCL)) < 0)
		dprintf(2, "msg_commerce.c - msg_commerce_in_port_init: Failed to create message queue.\n");
	return id;
}

int msg_commerce_in_port_get_id()
{
	int id;
	if ((id = msgget(MSG_IN_PORT_KEY, 0)) < 0)
		dprintf(2, "msg_commerce.c - msg_commerce_in_port_get_id: Failed to get message queue id.\n");

	return id;
}

int msg_commerce_out_port_get_id()
{
	int id;
	if ((id = msgget(MSG_OUT_PORT_KEY, 0)) < 0)
		dprintf(2, "msg_commerce.c - msg_commerce_out_port_get_id: Failed to get message queue id.\n");
	return id;
}

commerce_msg_t msg_commerce_create(long sender_id, long receiver_id, int cargo_id, int quantity, int expiry_date, int status)
{
	commerce_msg_t res;
	res.sender = sender_id;
	res.receiver = receiver_id;
	res.cargo_id = cargo_id;
	res.quantity = quantity;
	res.expiry_date = expiry_date;
	res.status = status;
	return res;
}

void msg_commerce_send(int queue_id, commerce_msg_t *msg)
{
	int ret;
	do {
		ret = msgsnd(queue_id, msg, MSG_SIZE, 0);
	} while (ret < 0);
}
