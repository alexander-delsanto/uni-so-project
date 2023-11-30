#ifndef OS_PROJECT_TYPES_H
#define OS_PROJECT_TYPES_H

/**
 * @brief Type definition for boolean values.
 *
 * TRUE 1 | FALSE 0
 */
typedef int bool_t;
#define TRUE 1
#define FALSE 0

/**
 * @brief Represents coordinates.
 */
struct coord {
	double x, y;
};

#endif
