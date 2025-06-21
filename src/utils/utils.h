#ifndef UTILS_H
#define UTILS_H

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "map.h"
#include "memory_array.h"

void free_all(char **program, struct memory_array *array, map *breakpoints);

int parse_array_size(char *array_size_string);

char **getlines(char *filename);

void putescchar(unsigned char c);

#endif /* UTILS_H */
