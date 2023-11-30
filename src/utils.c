#define _GNU_SOURCE

#include <time.h>
#include <errno.h>

#include "include/utils.h"

static struct timespec get_timespec(double time_required)
{
	struct timespec res;
	res.tv_sec = (time_t)time_required;
	res.tv_nsec = ((suseconds_t)((time_required - (double)res.tv_sec) *1e9));

	return res;
}

void convert_and_sleep(double time_required)
{
	struct timespec sleep_time, remaining_time;

	if (time_required <= 0)
		return;

	sleep_time = get_timespec(time_required);
	do {
		errno = EXIT_SUCCESS;
		nanosleep(&sleep_time, &remaining_time);
		sleep_time = remaining_time;
	} while (errno == EINTR);
}
