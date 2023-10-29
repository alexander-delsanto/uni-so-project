#ifndef SHM_H
#define SHM_H

struct SHM {
	int fd;
	char *name;
	void *data;
};
typedef struct SHM shm;

shm *shm_create(char *name, int oflag, mode_t mode);
void shm_delete(shm *shared);
void shm_write(shm *shared, void *data, size_t size, int flags);
void *shm_read(shm *shared);

#endif //SHM_H
