#define _GNU_SOURCE

#include <time.h>

#include "include/utils.h"

void convert_and_sleep(double time_required)
{
	struct timespec sleep_time;

	if (time_required <= 0)
		return;

	/* conversion in timespec to invoke nanosleep() */
	sleep_time.tv_sec = (time_t)time_required;
	sleep_time.tv_nsec =
		(long)((time_required - (double)sleep_time.tv_sec) *
		       1e9);

	nanosleep(&sleep_time, NULL);
}
