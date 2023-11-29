#include <stdio.h>
#include <stdlib.h>

#include "../_src/header/_cargo_list.h"

int main(int argc, char *argv[])
{
	o_list_t *list, *out;

	list = cargo_list_create();

	cargo_list_add(list, 5, 10);
	cargo_list_add(list, 5, 2);
	cargo_list_add(list, 6, 2);
	cargo_list_add(list, 6, 4);

	printf("All:\n");
	cargo_list_print_all(list);

	printf("Remove expired:\n");
	cargo_list_remove_expired(list, 2);
	cargo_list_print_all(list);

	printf("After popss:\n");
	out = cargo_list_pop_needed(list, 8);
	cargo_list_print_all(list);

	printf("Popped:\n");
	cargo_list_print_all(out);

	cargo_list_delete(out);
	cargo_list_delete(list);

	return EXIT_SUCCESS;
}
