#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#include "../lib/semaphore.h"

#include "include/const.h"
#include "include/shm_general.h"
#include "include/shm_port.h"
#include "include/shm_ship.h"
#include "include/shm_cargo.h"
#include "include/utils.h"
#include "include/shm_offer_demand.h"
#include "include/cargo_list.h"
#include "include/msg_commerce.h"

#define GET_DISTANCE(dest)\
	(sqrt(pow(dest.x - shm_ship_get_coords(state.ship, state.id).x, 2) + pow(dest.y - shm_ship_get_coords(state.ship, state.id).y, 2)))

void signal_handler(int signal);

void init_location(void);
int pick_first_destination_port(void);
void trade(void);
int sell(int cargo_type);
int ship_sell(int amount_to_sell, int cargo_type);
int buy(int cargo_type);
int ship_buy(int cargo_type, int amount_to_buy, int expiration_date);
void move(int port_id);

void close_all(void);
void loop(void);
int find_new_destination_port(void);

struct state {
	int id;
	shm_general_t *general;
	shm_port_t *port;
	shm_ship_t *ship;
	shm_cargo_t *cargo;

	shm_demand_t *demand;
	shm_offer_t *offer;
	o_list_t **cargo_hold;

	int curr_port_id;
};

struct state state;

int main(int argc, char *argv[])
{
	struct sigaction sa;
	int i;
	sigset_t mask;
	bzero(&state, sizeof (struct state));
	bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_handler;

	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);

	state.id = (int)strtol(argv[1], NULL, 10);
	shm_general_attach(&state.general);
	state.port = shm_port_attach(state.general);
	state.ship = shm_ship_attach(state.general);
	state.cargo = shm_cargo_attach(state.general);
	state.demand = shm_demand_attach(state.general);
	state.offer = shm_offer_attach(state.general);

	state.cargo_hold = malloc(8 * get_merci(state.general));
	for (i = 0; i < get_merci(state.general); i++) {
		state.cargo_hold[i] = cargo_list_create();
	}

	srand(time(NULL) * getpid());
	init_location();

	sigemptyset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGSTORM, &sa, NULL);
	sigaction(SIGMAELSTROM, &sa, NULL);

	sem_execute_semop(sem_port_init_get_id(state.general), 0, 0, 0);
	sem_execute_semop(sem_start_get_id(state.general), 0, 0, 0);
	loop();
}

void loop(void)
{
	int id_dest_port;

	id_dest_port = pick_first_destination_port();
	move(id_dest_port);
	trade();
	while (1) {
		id_dest_port = find_new_destination_port();
		move(id_dest_port);
		trade();
	}
}

/**
 * @brief initializes ship's location.
 */
void init_location(void)
{
	struct coord coords;
	/* generate a random location on the map */
	coords.x = RANDOM_DOUBLE(0, get_lato(state.general));
	coords.y = RANDOM_DOUBLE(0, get_lato(state.general));

	shm_ship_set_coords(state.ship, state.id, coords);
	shm_ship_set_is_moving(state.ship, state.id, TRUE);
}

int pick_first_destination_port(void)
{
	int target_port;
	target_port = RANDOM_INTEGER(0, get_porti(state.general) - 1);
	return target_port;
}

/**
 * @brief simulates the movement of the ship and updates the location.
 */
void move(int port_id)
{
	struct coord dest_coords;
	double time_required;

	dest_coords = shm_port_get_coordinates(state.port, port_id);
	shm_ship_set_is_moving(state.ship, state.id, TRUE);
	/* calculate time required to arrive (in days) */
	time_required = GET_DISTANCE(dest_coords) / get_speed(state.general);
	convert_and_sleep(time_required);
	/* set new location */
	shm_ship_set_coords(state.ship, state.id, dest_coords);
	shm_ship_set_is_moving(state.ship, state.id, FALSE);
	state.curr_port_id = port_id;
}

int find_new_destination_port(void)
{
	int cargo_type;
	int port, best_port = -1;
	int n_ports;
	int port_demand;
	int sale_amount, sale_best_amount = 0;
	int amount_not_expired;
	double time_required, best_time;

	n_ports = get_porti(state.general);
	for (port = 0; port < n_ports; port++) {
		if (port == state.curr_port_id) continue;

		/* Check port distance */
		time_required = GET_DISTANCE(shm_port_get_coordinates(state.port, port)) / get_speed(state.general);

		sale_amount = 0;
		for (cargo_type = 0; cargo_type < get_merci(state.general); cargo_type++) {
			amount_not_expired = cargo_list_get_not_expired_by_day(state.cargo_hold[cargo_type], get_current_day(state.general) + (int) time_required);
			port_demand = shm_demand_get_quantity(state.general, state.demand, port, cargo_type);
			sale_amount += MIN(amount_not_expired, port_demand);
		}

		if (best_port == -1 || sale_amount > sale_best_amount
		    || (sale_best_amount == sale_amount && time_required < best_time)) {
			best_port = port;
			sale_best_amount = sale_amount;
			best_time = time_required;
		}
	}
	return best_port;
}

void trade(void)
{
	int i, n_cargo, cargo_type, sem_docks_id;
	int n_expired;
	int load_speed, tons_moved;
	sigset_t mask;
	load_speed = get_load_speed(state.general);
	n_cargo = get_merci(state.general);

	sem_docks_id = shm_port_get_sem_docks_id(state.port);
	sigemptyset(&mask);
	sigaddset(&mask, SIGMAELSTROM);

	/* Requesting dock */
	sem_execute_semop(sem_docks_id, state.curr_port_id, -1, SEM_UNDO);
	shm_ship_set_is_at_dock(state.ship, state.id, TRUE);

	/* Selling */
	if (shm_ship_get_capacity(state.ship, state.id) < get_capacity(state.general)) {
		for (i = 0; i < n_cargo; i++) {
			shm_ship_remove_expired(state.general, state.ship, state.cargo, state.cargo_hold, state.id);
			sigprocmask(SIG_BLOCK, &mask, NULL);
			tons_moved = sell(i);
			sigprocmask(SIG_UNBLOCK, &mask, NULL);
			if (tons_moved > 0)
				convert_and_sleep(tons_moved / (double)load_speed);
		}
	}

	/* Buying */
	cargo_type = RANDOM_INTEGER(0, n_cargo -1);
	for (i = 0; i < n_cargo; i++) {
		if (shm_ship_get_capacity(state.ship, state.id) <= 0) break;
		shm_ship_remove_expired(state.general, state.ship, state.cargo, state.cargo_hold, state.id);
		cargo_type = (cargo_type + i) % n_cargo;
		if (shm_offer_get_quantity(state.general, state.offer, state.curr_port_id, cargo_type) <= 0)
			continue;
		sigprocmask(SIG_BLOCK, &mask, NULL);
		tons_moved = buy(cargo_type);
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
		if (tons_moved > 0)
			convert_and_sleep(tons_moved / (double)load_speed);
	}

	/* Releasing dock */
	sem_execute_semop(sem_docks_id, state.curr_port_id, 1, SEM_UNDO);
	shm_ship_set_is_at_dock(state.ship, state.id, FALSE);

}

int sell(int cargo_type)
{
	struct commerce_msg msg;
	int available_in_ship, port_demand, amount_to_sell;
	int quantity, status;

	available_in_ship = 0;
	/*available_in_ship = cargo_list_get_quantity_by_id(state.cargo_hold, cargo_type);*/
	available_in_ship += cargo_list_get_quantity(state.cargo_hold[cargo_type]);

	port_demand = shm_demand_get_quantity(state.general, state.demand, state.curr_port_id, cargo_type);
	amount_to_sell = MIN(available_in_ship, port_demand);
	if (amount_to_sell <= 0) return 0;

	msg = msg_commerce_create(state.curr_port_id, state.id, cargo_type, amount_to_sell, -1, STATUS_SELL);
	msg_commerce_send(msg_in_get_id(state.general), &msg);
	msg_commerce_receive(msg_out_get_id(state.general), state.id, NULL, NULL, &quantity, NULL, &status, TRUE);

	if (status == STATUS_ACCEPTED && quantity > 0) {
		return ship_sell(quantity, cargo_type);
	}
	return 0;
}

int ship_sell(int amount_to_sell, int cargo_type)
{
	int tons_sold;
	cargo_list_pop_needed(state.cargo_hold[cargo_type], amount_to_sell);

	tons_sold = amount_to_sell * shm_cargo_get_size(state.cargo, cargo_type);
	shm_ship_update_capacity(state.ship, state.id, tons_sold);
	shm_cargo_update_dump_available_on_ship(state.cargo, cargo_type, -amount_to_sell, sem_cargo_get_id(state.general));
	return tons_sold;
}

int buy(int cargo_type)
{
	struct commerce_msg msg;
	int msg_out_id;
	int available_ship_capacity, n_in_capacity, amount_to_buy;
	int available_in_port;
	int tons_bought = 0;
	int quantity, expiration_date, status;
	available_in_port = shm_offer_get_quantity(state.general, state.offer, state.curr_port_id, cargo_type);
	available_ship_capacity = shm_ship_get_capacity(state.ship, state.id);
	n_in_capacity = available_ship_capacity / shm_cargo_get_size(state.cargo, cargo_type);
	if (n_in_capacity <= 0) return 0;
	amount_to_buy = RANDOM_INTEGER(1, MIN(n_in_capacity, available_in_port));
	msg = msg_commerce_create(state.curr_port_id, state.id, cargo_type, amount_to_buy, -1, STATUS_BUY);
	msg_commerce_send(msg_in_get_id(state.general), &msg);

	msg_out_id = msg_out_get_id(state.general);
	do {
		msg_commerce_receive(msg_out_id, state.id, NULL, NULL, &quantity, &expiration_date, &status, TRUE);
		if (status == STATUS_PARTIAL || status == STATUS_ACCEPTED) {
			tons_bought += ship_buy(cargo_type, quantity, expiration_date);
		}
	} while (status == STATUS_PARTIAL);

	return tons_bought;
}

int ship_buy(int cargo_type, int amount_to_buy, int expiration_date)
{
	int tons_bought;
	cargo_list_add(state.cargo_hold[cargo_type], amount_to_buy, expiration_date);

	tons_bought = amount_to_buy * shm_cargo_get_size(state.cargo, cargo_type);
	shm_ship_update_capacity(state.ship, state.id, -tons_bought);
	shm_cargo_update_dump_available_on_ship(state.cargo, cargo_type, amount_to_buy, sem_cargo_get_id(state.general));

	return tons_bought;

}

void signal_handler(int signal)
{
	switch (signal) {
	case SIGSTORM:
		shm_ship_set_dump_had_storm(state.ship, state.id);
		convert_and_sleep(get_storm_duration(state.general) / 24.0);
		break;
	case SIGMAELSTROM:
		shm_ship_set_had_maelstrom(state.ship, state.id);
		shm_ship_remove_expired(state.general, state.ship, state.cargo, state.cargo_hold, state.id);
		shm_ship_remove_cargo_maelstrom(state.general, state.ship, state.cargo, state.cargo_hold, state.id);
		close_all();
	case SIGSEGV:
		dprintf(1, "ship.c: id: %d: Received SIGSEGV signal.\n", state.id);
	case SIGINT:
		close_all();
	}
}

void close_all(void)
{
	int i;

	for (i = 0; i < get_merci(state.general); i++) {
		cargo_list_delete(state.cargo_hold[i]);
	}
	free(state.cargo_hold);

	shm_ship_set_is_dead(state.ship, state.id);
	shm_port_detach(state.port);
	shm_ship_detach(state.ship);
	shm_cargo_detach(state.cargo);
	shm_offer_detach(state.offer);
	shm_demand_detach(state.demand);
	shm_general_detach(state.general);
	exit(EXIT_SUCCESS);
}
