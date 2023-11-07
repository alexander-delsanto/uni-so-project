#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "header/ipc_utils.h"
#include "header/utils.h"
#include "header/shared_memory.h"

#define NUM_CONST 16

pid_t master_pid;
pid_t *children_pid = NULL;
int children_num = 0;

void create_children();
pid_t run_process(char *name, int index);
struct data_general read_constants_from_file(char *path);
void send_signal_to_children(int signal);
void signal_handler(int signal);
void close_all();

int main()
{
	struct sigaction sa;
	sigset_t mask;
	struct data_general read_data;

	master_pid = getpid();

	/* Signal handler initialization */
	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;

	sigfillset(&mask);
	sa.sa_mask = mask;
	sigaction(SIGALRM, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

	read_data = read_constants_from_file("../constants.txt");

	initialize_shm(&read_data);
	create_children();
	start_simulation();

	alarm(1);

	while(1) {
		pause();
	}
}

void create_children() {
	int i;
	children_pid = calloc(SO_NAVI + SO_PORTI + 1, sizeof(*children_pid));
	/* Running ports */
/*	for (i = 0; i < SO_PORTI; i++) {
		set_port_pid(i, run_process("./port", i));
	}*/
	/* Running ships */
	for (i = 0; i < SO_NAVI; i++) {
		set_ship_pid(i, run_process("./ship", i));
	}
	/* Running weather */
	/* run_process("./weather", 0);*/
}

pid_t run_process(char *name, int index)
{
	pid_t process_pid;
	char *args[3], buf[10];
	if((process_pid = fork()) == -1) {
		dprintf(2, "master.c: Error in fork.\n");
		close_all();
	} else if (process_pid == 0) {
		sprintf(buf, "%d", index);
		args[0] = name;
		args[1] = buf;
		args[2] = NULL;
		if (execve(name, args, NULL) == -1) {
			perror("execve");
			exit(EXIT_SUCCESS);
		}
	}

	children_pid[children_num] = process_pid;
	children_num++;
	return process_pid;
}

struct data_general read_constants_from_file(char *path)
{
	FILE *file;
	char c;
	int n_char, counter = 0;
	double value;
	struct data_general read_data;

	file = fopen(path, "r");
	if(file == NULL)
		close_all();
	while((n_char = fscanf(file, "%lf", &value)) != EOF){
		if(n_char != 0) {
			if(counter >= NUM_CONST) {
				fclose(file);
				close_all();
			}if (value <= 0) {
				fclose(file);
				close_all();
			}
			if (counter <= 0) {
				read_data.so_lato = value;
			} else {
				(&read_data.so_days)[counter - 1] = (int) value;
			}
			counter++;
		}

		fscanf(file, "%*[ \t]");
		if((c = fgetc(file)) == '#') {
			fscanf(file, "%*[^\n]");
		}else if(!(c >= '0' && c <= '9')) {
			fclose(file);
			close_all();
		}
	}
	fclose(file);
	return read_data;
}

void send_signal_to_children(int signal)
{
	int i;
	if(children_pid != NULL) {
		for (i = 0; i < children_num; i++) {
			kill(children_pid[i], signal);
		}
	}
}

void signal_handler(int signal)
{
	switch (signal){
	case SIGSEGV:
		dprintf(2, "master.c: Segmentation fault. Closing all.\n");
		close_all();
	case SIGTERM:
	case SIGINT:
		close_all();
	case SIGALRM:
		new_day();
		dprintf(1, "\n");
		send_signal_to_children(SIGDAY);
		alarm(1);
		break;
	}
}

void close_all()
{
	send_signal_to_children(SIGKILL);
	while (wait(NULL) > 0);

	free(children_pid);
	delete_all_shm();
	exit(0);
}
