#include "map.h"

map *map_add(map *m, ssize_t key, ssize_t val)
{
    struct list **l = map_get(m, key);
    if (l)
    {
        list_add(*l, val);
        return m;
    }

    map *new_map = malloc(sizeof(map));
    if (new_map)
    {
        new_map->next = m;
        new_map->key = key;
        new_map->values = list_add(NULL, val);
        return new_map;
    }

    return m;
}

struct list **map_get(map *m, ssize_t key)
{
    if (!m)
        return NULL;
    if (m->key == key)
        return &(m->values);
    return map_get(m->next, key);
}

map *map_remove(map *m, ssize_t e)
{
    if (!m)
        return NULL;
    if (m->key == e)
    {
        map *rest = m->next;
        list_destroy(m->values);
        return rest;
    }
    m->next = map_remove(m->next, e);
    return m;
}

void map_destroy(map *m)
{
    if (!m)
        return;

    map_destroy(m->next);
    list_destroy(m->values);
    free(m);
}
