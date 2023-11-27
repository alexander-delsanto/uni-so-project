#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "include/const.h"
#include "include/sem.h"
#include "include/shm_general.h"
#include "include/types.h"
#include "include/utils.h"
#include "include/shm_port.h"
#include "include/shm_offer_demand.h"
#include "include/cargo_list.h"
#include "include/msg_commerce.h"

struct state {
	int id;
	shm_general_t *general;
	shm_port_t *port;

	shm_offer_t *offer;
	shm_demand_t *demand;
	o_list_t *cargo;

	int current_day;
};

void signal_handler(int signal);
void signal_handler_init(void);
void loop(void);

void generate_coordinates(void);
void generate_docks(void);
void handle_message(void);

void close_all(void);

struct state state;

int main(int argc, char *argv[])
{
	bzero(&state, sizeof(struct state));
	signal_handler_init();

	state.id = (int)strtol(argv[1], NULL, 10);

	general_shm_attach(&state.general);
	if (state.general == NULL) {
		close_all();
		exit(1);
	}
	state.port = port_shm_attach(state.general);
	state.offer = offer_shm_ports_init(state.general);
	state.demand = demand_shm_init(state.general);

	srand(time(NULL) * getpid());
	generate_coordinates();
	generate_docks();

	sem_execute_semop(get_sem_port_init_id(), 0, -1, 0);
	sem_execute_semop(get_sem_start_id(), 0, 0, 0);

	loop();
}

void loop(void)
{
	int *expired;
	int i, day;

	while (1) {
		day = get_current_day(state.general);
		if (state.current_day < day) {
			/* TODO: new day operations */
			state.current_day = day;
			/* Dumping expired stuff */
			expired = cargo_list_remove_expired(state.cargo,
							    state.general);
			for (i = 0; i < get_merci(state.general); i++) {
				port_shm_dump_expired_add(state.port, state.id,
							  i, expired[i]);
			}
			/* Generation of new demand/offer */
			offer_demand_shm_generate(state.offer, state.demand,
						  state.cargo, state.id,
						  state.general);
		}
		handle_message();
	}
}

void handle_message(void)
{
	shm_offer_t *order;
	o_list_t *order_expires;

	struct node_msg *exp_node;

	struct commerce_msg msg;
	int tmp_quantity;
	int sender_id, cargo_id, quantity, expiry_date, capacity, status;

	if (!msg_commerce_receive(get_msg_in_id(state.general), state.id,
				  &sender_id, &cargo_id, &quantity,
				  &expiry_date, &capacity, &status, FALSE)) {
		return;
	}
	switch (status) {
	case STATUS_REQUEST:
		tmp_quantity = demand_shm_get(state.demand, state.id, cargo_id);
		if (tmp_quantity == 0) {
			msg = msg_commerce_create(sender_id, state.id, cargo_id,
						  tmp_quantity, 0, 0,
						  STATUS_REFUSED);
		} else if (tmp_quantity >= quantity) {
			msg = msg_commerce_create(sender_id, state.id, cargo_id,
						  quantity, 0, 0,
						  STATUS_ACCEPTED);
			demand_shm_remove(state.demand, state.id, cargo_id,
					  quantity);
		} else {
			msg = msg_commerce_create(sender_id, state.id, cargo_id,
						  tmp_quantity, 0, 0,
						  STATUS_ACCEPTED);
			demand_shm_remove(state.demand, state.id, cargo_id,
					  tmp_quantity);
		}

		msg_commerce_send(get_msg_out_id(state.general), &msg);
		break;
	case STATUS_SELLING:
		port_shm_dump_received_add(state.port, state.id, cargo_id,
					   quantity);
		break;
	case STATUS_MISSING:
	case STATUS_DEAD:
		demand_shm_add(state.demand, state.id, cargo_id, quantity);
		break;
	case STATUS_LOAD_REQUEST:
		/* Getting order from capacity */
		order = offer_shm_get_order(state.offer, state.general,
					    state.id, capacity);
		/* Getting order expires */
		order_expires = offer_shm_get_order_expires(state.cargo, order,
							    state.general);
		while ((exp_node = cargo_list_pop_order(
				order_expires, state.general)) != NULL) {
			/* Sending items */
			msg = msg_commerce_create(sender_id, state.id,
						  exp_node->id,
						  exp_node->quantity,
						  exp_node->expire, 0,
						  STATUS_LOAD_ACCEPTED);
			msg_commerce_send(get_msg_out_id(state.general), &msg);
			/* Updating dump of sent items */
			port_shm_dump_sent_add(state.port, state.id,
					       exp_node->id,
					       exp_node->quantity);

			free(exp_node);
		}

		cargo_list_delete(order_expires, state.general);
		offer_shm_delete(order);
		break;
	default:
		break;
	}
}

void generate_coordinates(void)
{
	struct coord coordinates;
	double max;

	max = get_lato(state.general);

	switch (state.id) {
	case 0:
		coordinates.x = 0;
		coordinates.y = 0;
		break;
	case 1:
		coordinates.x = 0;
		coordinates.y = max;
		break;
	case 2:
		coordinates.x = max;
		coordinates.y = 0;
		break;
	case 3:
		coordinates.x = max;
		coordinates.y = max;
		break;
	default:
		coordinates.x = RANDOM_DOUBLE(0, max);
		coordinates.y = RANDOM_DOUBLE(0, max);
		break;
	}

	port_shm_set_coordinates(state.port, state.id, coordinates);
}

void generate_docks(void)
{
	int n;

	n = RANDOM_INTEGER(1, get_banchine(state.general));
	/* TODO: gen semaphore
	state.sem_id = sem_create(100, n);*/

	port_shm_set_docks(state.port, state.id, n);
}

void signal_handler_init(void)
{
	static struct sigaction sa;
	sigset_t mask;

	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;

	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

	sigaction(SIGDAY, &sa, NULL);
	sigaction(SIGSWELL, &sa, NULL);
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
		break;
	case SIGSWELL:
		port_shm_set_is_in_swell(state.port, state.id, TRUE);
		convert_and_sleep(get_swell_duration(state.general) / 24.0);
		port_shm_set_is_in_swell(state.port, state.id, FALSE);
		break;
	case SIGSEGV:
		dprintf(1, "Received SIGSEGV signal.\n");
		dprintf(2, "ship.c: id: %d: Segmentation fault. Terminating.\n",
			state.id);
	case SIGINT:
		close_all();
	default:
		break;
	}
}

void close_all(void)
{
	port_shm_detach(state.port);
	general_shm_detach(state.general);
	exit(EXIT_SUCCESS);
}
