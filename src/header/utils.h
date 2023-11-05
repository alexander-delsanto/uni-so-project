#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>

typedef int bool_t;
#define TRUE 1
#define FALSE 0

/**
 * @return a random double between min and max.
 */
#define RANDOM_DOUBLE(min, max)\
	(rand() / (double)RAND_MAX * (max - min) + (min))

#define SECONDS_IN_DAY 86400 /* constant for conversion in seconds */
#define NANOSECONDS_IN_SECOND 1000000000L /* constant for conversion to nanoseconds */

#endif
