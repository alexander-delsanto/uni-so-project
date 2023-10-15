#define _GNU_SOURCE
#include "header/utils.h"
#include "header/shared_memory.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

pid_t master_pid;
pid_t *children_pid;
int children_num;

int main()
{
	struct sigaction sa;
	sigset_t masked_signals;

	master_pid = getpid();

	/* Signal handler initialization */
	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;
	sigfillset(&masked_signals);
	sa.sa_mask = masked_signals;
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);



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
