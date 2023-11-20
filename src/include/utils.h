#ifndef OS_PROJECT_UTILS_H
#define OS_PROJECT_UTILS_H

#define RANDOM_INTEGER(min, max) ((random() % ((max) - (min) + 1)) + (min))

/* TODO: la rand dipende da stdlib, è un side effect che non venga inclusa da sè */
#define RANDOM_DOUBLE(min, max) \
	(random() / (double)RAND_MAX * (max - min) + (min))

#define RANDOM_BOOL() ((bool_t)random() % 2)
#endif
