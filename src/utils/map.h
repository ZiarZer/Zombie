#ifndef MAP_H
#define MAP_H

#include "list.h"

struct pair_list
{
    ssize_t key;
    struct list *values;

    struct pair_list *next;
};

typedef struct pair_list map;

map *map_add(map *m, ssize_t key, ssize_t val);

struct list **map_get(map *m, ssize_t key);

map *map_remove(map *m, ssize_t e);

void map_destroy(map *m);

#endif /* !MAP_H */
