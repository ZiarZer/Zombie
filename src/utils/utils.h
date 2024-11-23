#ifndef UTILS_H
#define UTILS_H

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <sys/stat.h>

#include "bracket_pair.h"
#include "map.h"

size_t size_t_max(size_t a, size_t b);

void free_all(char **program, struct bracket_pair *brackets, unsigned char *array, map *breakpoints);

int check_array_size(char *array_size_string);

char **getlines(char *filename);

void putescchar(unsigned char c);

#endif /* UTILS_H */
