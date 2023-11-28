#ifndef OS_PROJECT_UTILS_H
#define OS_PROJECT_UTILS_H

/**
 * @return a random integer between min and max (included).
 */
#define RANDOM_INTEGER(min, max) ((int)((random() % ((max) - (min) + 1)) + (min)))

/**
 * @return a random double between min and max (included).
 */
/* TODO: la rand dipende da stdlib, è un side effect che non venga inclusa da sè */
#define RANDOM_DOUBLE(min, max) \
	(random() / (double)RAND_MAX * (max - min) + (min))
/**
 * @return Random boolean value.
 */
#define RANDOM_BOOL() ((bool_t)random() % 2)

/**
 * @return the minimum value between x and y.
 */
#define MIN(x,y) ((x) < (y) ? (x) : (y))

/**
 * @brief converts time data to timespec and calls nanosleep().
 *
 * @param time_required time required to terminate.
 */
void convert_and_sleep(double time_required);

#endif
