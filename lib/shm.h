#ifndef SHM_H
#define SHM_H

typedef struct SHM shm;

shm *shm_create(key_t key, size_t length);
void shm_delete(shm *shared);

unsigned int shm_getid(shm *shared);
shm *shm_open(unsigned int id, size_t length);
void shm_close(shm *shared);

void *shm_read(shm *shared);
void shm_write(shm *shared, const void *data, size_t length);

#endif
