#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "../lib/semaphore.h"

#include "include/const.h"
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
	o_list_t **cargo_hold;

	int current_day;
};

void signal_handler(int signal);
void signal_handler_init(void);
void loop(void);

void respond_ship_msg(int ship_id, int cargo_type, int amount, int status);

void generate_coordinates(void);

void close_all(void);

struct state state;

int main(int argc, char *argv[])
{
	int i;
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
	state.offer = shm_offer_attach(state.general);
	state.demand = shm_demand_attach(state.general);
	state.cargo_hold = malloc(sizeof(state.cargo_hold) * get_merci(state.general));
	for (i = 0; i < get_merci(state.general); i++) {
		state.cargo_hold[i] = cargo_list_create();
	}


	srand(time(NULL) * getpid());
	generate_coordinates();

	sem_execute_semop(sem_port_init_get_id(state.general), 0, -1, 0);
	sem_execute_semop(sem_start_get_id(state.general), 0, 0, 0);

	loop();
}

void loop(void)
{
	int msg_in_id = msg_in_get_id(state.general);
	int ship_id, needed_type, needed_amount, status;
	int day;

	shm_offer_demand_generate(state.offer, state.demand, state.cargo_hold, state.id, state.cargo, state.general);
	while (1) {
		day = get_current_day(state.general);
		if (state.current_day < day) {
			state.current_day = day;
			/* Dumping expired stuff */
			shm_port_remove_expired(state.general, state.port, state.offer, state.cargo, state.cargo_hold, state.id);
			shm_port_update_dump_cargo_available(state.general, state.port, state.offer, state.id);
			/* Generation of new demand/offer */
			shm_offer_demand_generate(state.offer, state.demand, state.cargo_hold, state.id, state.cargo, state.general);
		}
		if (msg_commerce_receive(msg_in_id, state.id, &ship_id, &needed_type, &needed_amount, NULL, &status, FALSE) == TRUE) {
			respond_ship_msg(ship_id, needed_type, needed_amount, status);
		}
	}
}


void respond_ship_msg(int ship_id, int cargo_type, int amount, int status)
{
	o_list_t *cargo;
	struct commerce_msg msg;
	int msg_out_id = msg_out_get_id(state.general);
	int port_amount;
	int quantity, expiration_date;

	int exchanged_amount;

	if (status == STATUS_SELL) { /* Port is buying */
		port_amount = shm_demand_get_quantity(state.general, state.demand, state.id, cargo_type);
		exchanged_amount = MIN(amount, port_amount);
		shm_demand_remove_quantity(state.demand, state.general, state.id, cargo_type, exchanged_amount);
		msg = msg_commerce_create(ship_id, state.id, cargo_type, exchanged_amount, -1, STATUS_ACCEPTED);
		msg_commerce_send(msg_out_id, &msg);
		shm_cargo_update_dump_received_in_port(state.cargo, cargo_type, exchanged_amount, sem_cargo_get_id(state.general));
		shm_port_update_dump_cargo_received(state.port, state.id, exchanged_amount);

	} else if (status == STATUS_BUY) { /* Port is selling */
		port_amount = shm_offer_get_quantity(state.general, state.offer, state.id, cargo_type);
		if (port_amount <= 0) {
			msg = msg_commerce_create(ship_id, state.id, -1, -1, -1, STATUS_REFUSED);
			msg_commerce_send(msg_out_id, &msg);
			return;
		}
		exchanged_amount = MIN(amount, port_amount);
		shm_offer_remove_quantity(state.offer, state.general, state.id, cargo_type, exchanged_amount);
		shm_cargo_update_dump_available_in_port(state.cargo, cargo_type, -exchanged_amount, sem_cargo_get_id(state.general));
		shm_port_update_dump_cargo_shipped(state.port, state.id, exchanged_amount);
		cargo = cargo_list_pop_needed(state.cargo_hold[cargo_type], exchanged_amount);
		while (exchanged_amount > 0) {
			cargo_list_pop(cargo, &quantity, &expiration_date);
			exchanged_amount -= quantity;
			status = exchanged_amount <= 0 ? STATUS_ACCEPTED : STATUS_PARTIAL;
			msg = msg_commerce_create(ship_id, state.id, cargo_type, quantity, expiration_date, status);
			msg_commerce_send(msg_out_id, &msg);
		}
		shm_port_update_dump_cargo_available(state.general, state.port, state.offer, state.id);
	} else {
		msg = msg_commerce_create(ship_id, state.id, -1, -1, -1, STATUS_REFUSED);
		msg_commerce_send(msg_out_id, &msg);
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

void signal_handler_init(void)
{
	static struct sigaction sa;

	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;

	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSWELL, &sa, NULL);
	sigaction(SIGDAY, &sa, NULL);
}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGDAY:
		break;
	case SIGSWELL:
		shm_port_set_is_in_swell(state.port, state.id, TRUE);
		convert_and_sleep(get_swell_duration(state.general) / 24.0);
		shm_port_set_is_in_swell(state.port, state.id, FALSE);
		break;
	case SIGSEGV:
		dprintf(1, "port.c: id: %d: Received SIGSEGV signal.\n", state.id);
	case SIGINT:
		close_all();
	default:
		break;
	}
}

void close_all(void)
{
	int i;

	for (i = 0; i < get_merci(state.general); i++) {
		cargo_list_delete(state.cargo_hold[i]);
	}
	free(state.cargo_hold);
	shm_port_detach(state.port);
	shm_cargo_detach(state.cargo);
	shm_offer_detach(state.offer);
	shm_demand_detach(state.demand);
	shm_general_detach(state.general);
	exit(EXIT_SUCCESS);
}
