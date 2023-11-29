#ifndef OS_PROJECT_MSG_COMMERCE_H
#define OS_PROJECT_MSG_COMMERCE_H

#include <sys/msg.h>

#include "types.h"

enum status {
	STATUS_DEAD = -1,
	/* For port to ship messages */
	STATUS_ACCEPTED,
	STATUS_REFUSED,
	STATUS_LOAD_ACCEPTED,
	/* For ship to port messages */
	STATUS_REQUEST,
	STATUS_MISSING,
	STATUS_LOAD_REQUEST,
	STATUS_SELLING
};

struct commerce_msg {
	long receiver;
	long sender;
	int cargo_id;
	int quantity;
	int expiry_date;

	int status;
};

/**
 * @brief Initialize an incoming message queue.
 * @return The identifier of the initialized message queue.
 */
int msg_commerce_in_port_init(void);
/**
 * @brief Initialize an outgoing message queue.
 * @return The identifier of the initialized message queue.
 */
int msg_commerce_out_port_init(void);

/**
 * @brief Get the identifier of an incoming message queue.
 * @return The identifier of the existing message queue.
 */
int msg_commerce_in_port_get_id(void);
/**
 * @brief Get the identifier of an outgoing message queue.
 * @return The identifier of the existing message queue.
 */
int msg_commerce_out_port_get_id(void);

/**
 * @brief Create a commerce message.
 * @param receiver_id Identifier of the receiver.
 * @param sender_id Identifier of the sender.
 * @param cargo_id Identifier of the cargo.
 * @param quantity Quantity of the cargo.
 * @param expiry_date Expiry date of the cargo.
 * @param status Status of the cargo.
 * @return The created message.
 */
struct commerce_msg msg_commerce_create(long receiver_id, long sender_id,
					int cargo_id, int quantity,
					int expiry_date, int status);

/**
 * @brief Send a commerce message to a message queue.
 * @param queue_id Identifier of the message queue.
 * @param msg Pointer to the commerce message to be sent.
 */
void msg_commerce_send(int queue_id, struct commerce_msg *msg);

/**
 * @brief Receive a commerce message from a message queue.
 * @param queue_id Identifier of the message queue.
 * @param type Type of the message to receive.
 * @param sender_id Pointer to store the sender identifier.
 * @param cargo_id Pointer to store the cargo identifier.
 * @param quantity Pointer to store the quantity of goods.
 * @param expiry_date Pointer to store the expiry date of goods.
 * @param status Pointer to store the status of the message.
 * @param restarting Flag indicating whether the system is restarting.
 * @return TRUE if the message is successfully received, FALSE otherwise.
 */
bool_t msg_commerce_receive(int queue_id, int type, int *sender_id,
			    int *cargo_id, int *quantity, int *expiry_date,
			    int *status, bool_t restarting);

#endif
