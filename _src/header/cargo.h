#ifndef OS_PROJECT_CARGO_H
#define OS_PROJECT_CARGO_H

typedef struct goods goods_t;
typedef struct cargo cargo_t;

cargo_t *cargo_generate(int ids, int fill);
void cargo_generate_goodies(cargo_t *cargo, int size, int life_min,
			    int life_max);
void cargo_update_expire(cargo_t *cargo);
void cargo_delete(cargo_t *cargo);

#endif //OS_PROJECT_CARGO_H
