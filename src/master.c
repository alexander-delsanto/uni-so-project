#define _GNU_SOURCE
#include "header/utils.h"
#include "header/shared_memory.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define NUM_CONST 16

pid_t master_pid;
pid_t *children_pid = NULL;
int children_num = 0;

data_general read_constants_from_file(char *path);
void signal_handler(int signal);
void close_all();

int main()
{
	struct sigaction sa;
	sigset_t masked_signals;
    struct data read_data;
    int i = 0;

    master_pid = getpid();

	/* Signal handler initialization */
	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;
	sigfillset(&masked_signals);
	sa.sa_mask = masked_signals;
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

    read_data = read_constants_from_file("../constants.txt");
    dprintf(1, "%lf\n", read_data.so_lato);
    for(i = 0; i < NUM_CONST - 1; i++) {
        dprintf(1, "%d\n", (&read_data.so_days)[i]);
    }


}

data_general read_constants_from_file(char *path)
{
	FILE *file;
    char c;
    int n_char, counter = 0;
    double value;
	struct data read_data;

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

void signal_handler(int signal)
{
	switch (signal){
	case SIGSEGV:
		close_all();
	case SIGTERM:
	case SIGINT:
		close_all();
	case SIGALRM:
		/* TODO */
		break;
	}
}

void close_all()
{
	/* TODO */
}
