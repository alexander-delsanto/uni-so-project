#define _GNU_SOURCE

#include <stdio.h>

#include "include/msg_commerce.h"
#include "include/const.h"

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
