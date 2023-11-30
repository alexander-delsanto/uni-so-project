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
static void metodo_stupido();

void respond_ship_msg(int ship_id, int cargo_type, int amount, int status);

void generate_coordinates(void);
void handle_message(void);

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
	state.offer = shm_offer_ports_init(state.general);
	state.demand = shm_demand_init(state.general);
	state.cargo_hold = malloc(8 * get_merci(state.general));
	for (i = 0; i < get_merci(state.general); i++) {
		state.cargo_hold[i] = cargo_list_create();
	}


	srand(time(NULL) * getpid());
	generate_coordinates();

	dprintf(1, "port %d: n_docks: %d, sem_docks_val: %d\n", state.id,
		shm_port_get_docks(state.port, state.id),
		sem_getval(shm_port_get_sem_docks_id(state.port), state.id));

	sem_execute_semop(sem_port_init_get_id(state.general), 0, -1, 0);
	sem_execute_semop(sem_start_get_id(state.general), 0, 0, 0);

	dprintf(1, "port %d: reached loop\n", state.id);
	loop();
}

void loop(void)
{
	int msg_in_id = msg_in_get_id(state.general);
	int ship_id, needed_type, needed_amount, status;
	int sem_dump_id;
	int qt_expired = 0;
	int i, day;
	int tot_expired = 0;

	int n_merci;
	int tot_demand;

	n_merci = get_merci(state.general);
	sem_dump_id = sem_dump_get_id(state.general);
	while (1) {
		day = get_current_day(state.general);
		if (state.current_day < day) {
			/*dprintf(1, "port %d: going from day %d to day %d\n", state.id, state.current_day, day);*/
			tot_demand = 0;
			state.current_day = day;
			/* Dumping expired stuff */
			for (i = 0; i < n_merci; i++) {
				qt_expired = cargo_list_remove_expired(state.cargo_hold[i], state.current_day);
				shm_offer_remove_quantity(state.offer, state.general, state.id, i, qt_expired);
				/* TODO dump expired */
			}

			/*tot_expired += qt_expired;*/
			shm_port_set_dump_cargo_available(state.port, state.id,
							  shm_offer_get_tot_quantity(state.general, state.offer, state.id));
			/* Generation of new demand/offer */
			shm_offer_demand_generate(state.offer, state.demand,
						  state.cargo_hold, state.id,
						  state.cargo, state.general);
/*			for (i = 0; i < n_merci; i++) {
				dprintf(1, "id cargo: %d:\n", i);
				cargo_list_print_all(state.cargo_hold[i]);
			}*/
/*			for (i = 0; i < n_merci; i++) {
				tot_demand += shm_demand_get_quantity(state.general, state.demand, state.id, i);
			}
			dprintf(1, "port %d: tot_expired: %d, tot_demand: %d, tot: %d\n", state.id, tot_expired, tot_demand, tot_expired + tot_demand);*/
		}
		if (msg_commerce_receive(msg_in_id, state.id, &ship_id, &needed_type, &needed_amount, NULL, &status, FALSE) == TRUE) {
			dprintf(1, "port %d: got message from ship %d with status %d requesting %d of cargo %d\n", state.id, ship_id, status,
				needed_amount, needed_type);
			respond_ship_msg(ship_id, needed_type, needed_amount,
					 status);
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

	} else if (status == STATUS_BUY) { /* Port is selling */
		port_amount = shm_offer_get_quantity(state.general, state.offer, state.id, cargo_type);
		if (port_amount <= 0) {
			msg = msg_commerce_create(ship_id, state.id, -1, -1, -1, STATUS_REFUSED);
			msg_commerce_send(msg_out_id, &msg);
			return;
		}
		exchanged_amount = MIN(amount, port_amount);
		dprintf(1, "requested_amount: %d, port_amount: %d, exchanged_amount: %d\n", amount, port_amount, exchanged_amount);
		shm_offer_remove_quantity(state.offer, state.general, state.id, cargo_type, exchanged_amount);
		cargo = cargo_list_pop_needed(state.cargo_hold[cargo_type], exchanged_amount);
		if (cargo == NULL)
			dprintf(1, "cargo is null\n");
		while (exchanged_amount > 0) {
			cargo_list_pop(cargo, &quantity, &expiration_date);
			if (quantity != -1)
				dprintf(1, "port %d: - quantity: %d, exp_date: %d\n", state.id, quantity, expiration_date);
			exchanged_amount -= quantity;
			status = exchanged_amount <= 0 ? STATUS_ACCEPTED : STATUS_PARTIAL;
			msg = msg_commerce_create(ship_id, state.id, cargo_type, quantity, expiration_date, status);
			msg_commerce_send(msg_out_id, &msg);
		}
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
	shm_general_detach(state.general);
	exit(EXIT_SUCCESS);
}
