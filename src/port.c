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
#include "include/shm_cargo.h"
#include "include/shm_offer_demand.h"
#include "include/cargo_list.h"
#include "include/msg_commerce.h"

struct state {
	int id;
	shm_general_t *general;
	shm_port_t *port;
	shm_cargo_t *cargo;

	shm_offer_t *offer;
	shm_demand_t *demand;
	o_list_t *cargo_hold;

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

	shm_general_attach(&state.general);
	if (state.general == NULL) {
		close_all();
		exit(1);
	}
	state.port = shm_port_attach(state.general);
	state.cargo = shm_cargo_attach(state.general);
	state.offer = shm_offer_ports_init(state.general);
	state.demand = shm_demand_init(state.general);
	state.cargo_hold = cargo_list_create(state.general);

	srand(time(NULL) * getpid());
	generate_coordinates();
	generate_docks();

	dprintf(1, "port %d: n_docks: %d, sem_docks_val: %d\n", state.id,
		shm_port_get_docks(state.port, state.id),
		sem_getval(shm_port_get_sem_docks_id(state.port), state.id));

	sem_execute_semop(get_sem_port_init_id(), 0, -1, 0);
	sem_execute_semop(get_sem_start_id(), 0, 0, 0);

	dprintf(1, "port %d: reached loop\n", state.id);
	loop();
}

void loop(void)
{
	int qt_expired;
	int i, day;
	int tot_expired = 0;

	int n_merci;
	int tot_demand;
	n_merci = get_merci(state.general);
	while (1) {
		day = get_current_day(state.general);
		if (state.current_day < day) {
			state.current_day = day;
			/* Dumping expired stuff */
			qt_expired = cargo_list_remove_expired(state.cargo_hold,
							    state.general);
			tot_expired += qt_expired;
/*			for (i = 0; i < get_merci(state.general); i++) {
				port_shm_set_dump_expired(state.port, state.id,
							  i, *expired);
				dprintf(1, "\nCIAO\n");
			}*/
			/* Generation of new demand/offer */
			shm_offer_demand_generate(state.offer, state.demand,
						  state.cargo_hold, state.id,
						  state.cargo, state.general);
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
		tmp_quantity = shm_demand_get_quantity(state.general, state.demand, state.id, cargo_id);
		if (tmp_quantity == 0) {
			msg = msg_commerce_create(sender_id, state.id, cargo_id,
						  tmp_quantity, 0, 0,
						  STATUS_REFUSED);
		} else if (tmp_quantity >= quantity) {
			msg = msg_commerce_create(sender_id, state.id, cargo_id,
						  quantity, 0, 0,
						  STATUS_ACCEPTED);
			shm_demand_remove(state.demand, state.general, state.id, cargo_id,
					  quantity);
		} else {
			msg = msg_commerce_create(sender_id, state.id, cargo_id,
						  tmp_quantity, 0, 0,
						  STATUS_ACCEPTED);
			shm_demand_remove(state.demand, state.general, state.id, cargo_id,
					  tmp_quantity);
		}

		msg_commerce_send(get_msg_out_id(state.general), &msg);
		break;
	case STATUS_SELLING:

		break;
	case STATUS_MISSING:
	case STATUS_DEAD:
		shm_demand_set(state.demand, state.general, state.id, cargo_id, quantity);
		break;
	case STATUS_LOAD_REQUEST:
		/* Getting order from capacity */
		order = shm_offer_get_order(state.offer, state.general,
					    state.id, capacity);
		/* Getting order expires */
		order_expires = shm_offer_get_order_expires(state.cargo_hold, order,
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


			free(exp_node);
		}

		cargo_list_delete(order_expires, state.general);
		shm_offer_delete(order);
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

	shm_port_set_coordinates(state.port, state.id, coordinates);
}

void generate_docks(void)
{
	int sem_docks_id, n_docks;

	sem_docks_id = shm_port_get_sem_docks_id(state.port);
	n_docks = RANDOM_INTEGER(1, get_banchine(state.general));

	sem_setval(sem_docks_id, state.id, n_docks);
	shm_port_set_docks(state.port, state.id, n_docks);
}

void signal_handler_init(void)
{
	static struct sigaction sa;
	sigset_t mask;

	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;

	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

	sigaction(SIGSWELL, &sa, NULL);
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGSWELL:
		shm_port_set_is_in_swell(state.port, state.id, TRUE);
		convert_and_sleep(get_swell_duration(state.general) / 24.0);
		shm_port_set_is_in_swell(state.port, state.id, FALSE);
		break;
	case SIGSEGV:
		dprintf(1, "Received SIGSEGV signal.\n");
		dprintf(2, "port.c: id: %d: Segmentation fault. Terminating.\n",
			state.id);
	case SIGINT:
		close_all();
	default:
		break;
	}
}

void close_all(void)
{
	cargo_list_delete(state.cargo_hold, state.general);
	shm_port_detach(state.port);
	shm_cargo_detach(state.cargo);
	shm_general_detach(state.general);
	exit(EXIT_SUCCESS);
}
