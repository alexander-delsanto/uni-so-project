#ifndef SHM_H
#define SHM_H

#include <sys/shm.h>
#include <sys/stat.h>

typedef struct SHM shm;

shm *shm_create(key_t key, size_t length);
void shm_delete(shm *shared);

unsigned int shm_get_id(shm *shared);
shm *shm_attach(unsigned int id, size_t length);
void shm_detach(shm *shared);

void *shm_read(shm *shared);
void shm_write(shm *shared, const void *data, size_t length);

#endif
