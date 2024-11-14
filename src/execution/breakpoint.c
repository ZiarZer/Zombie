#include "breakpoint.h"

map *add_breakpoint(map *breakpoints, ssize_t i, ssize_t j)
{
    return map_add(breakpoints, i, j);
}

map *remove_breakpoint(map *breakpoints, ssize_t i, ssize_t j)
{
    struct list **values_ptr = map_get(breakpoints, i);
    if (values_ptr)
        *values_ptr = list_remove(*values_ptr, j);
    return breakpoints;
}

struct list *find_breakpoint(map *breakpoints, ssize_t i, ssize_t j)
{
    struct list **values_ptr = map_get(breakpoints, i);
    if (values_ptr)
        return list_find(*values_ptr, j);
    return NULL;
}
