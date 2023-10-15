#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void signal_handler(int signal);

int main() {
	struct sigaction sa;
	int i;

	printf("My pid is: %d\n", getpid());

	bzero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);

	while(1);
}


void signal_handler(int signal) {
	dprintf(2,"Can't kill me bro.\n");

}