#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include "map.h"

map *add_breakpoint(map *breakpoints, ssize_t i, ssize_t j);

map *remove_breakpoint(map *breakpoints, ssize_t i, ssize_t j);

struct list *find_breakpoint(map *breakpoints, ssize_t i, ssize_t j);

#endif /* !BREAKPOINT_H */
