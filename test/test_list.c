#include <stdio.h>
#include <stdlib.h>

#include "../src/include/list.h"

int main(int argc, char *argv[])
{
	o_list_t *list, *out;

	list = OrderedList();

	add(list, 5, 10);
	add(list, 5, 2);
	add(list, 6, 2);
	add(list, 6, 4);

	printf("All:\n");
	print_all(list);

	printf("Remove expired:\n");
	remove_expired(list, 2);
	print_all(list);

	printf("After popss:\n");
	out = pop_needed(list, 8);
	print_all(list);

	printf("Popped:\n");
	print_all(out);

	delete_list(out);
	delete_list(list);

	return EXIT_SUCCESS;
}
