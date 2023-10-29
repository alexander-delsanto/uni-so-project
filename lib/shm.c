#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "shm.h"

shm *shm_create(char *name, int oflag, mode_t mode)
{
	int fd;
	shm *shared;

	if (name == NULL) {
		fprintf(stderr, "Empty shmem name\n");
		return NULL;
	}
	if (name[1] != '/') {
		fprintf(stderr, "Malformed name, missing slash\n");
		return NULL;
	}

	shared = (shm *)malloc(sizeof(shm));
	if (shared == NULL) {
		fprintf(stderr, "Error: unable to allocate\n");
		return NULL;
	}

	shared->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
	if (shared->name == NULL) {
		// TODO error
		return NULL;
	}

	fd = shm_open(shared->name, oflag, mode);
	if (fd == -1) {
		fprintf(stderr, "Error: unable to open %s shmem\n",
			shared->name);
		return NULL;
	}

	shared->fd = fd;

	return shared;
}

void shm_delete(shm *shared)
{
	int result;

	if (shared == NULL) {
		fprintf(stderr, " SHM instance is NULL.\n");
		return;
	}

	result = shm_unlink(shared->name);
	if (result == -1) {
		fprintf(stderr, "Unable to close shmem, error: \n");
		// TODO errno?
		return;
	}
}

void shm_write(shm *shared, void *data, size_t size, int flags)
{
	void *ptr;

	/*
	 * NB: we should think about going back to non-posix shmem, since it's available
	 * only from C99 and greater.
	 */
	ftruncate(shared->fd, size);
	ptr = mmap(NULL, size, flags, MAP_SHARED, shared->fd, 0);
	if (data == MAP_FAILED) {
		fprintf(stderr, "Error: \n");
		return;
	}
	shared->data = data;
	memcpy(shared->data, data, size);
}

void *shm_read(shm *shared)
{
	return shared->data;
}
