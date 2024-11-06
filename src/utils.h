#ifndef UTILS_H
#define UTILS_H

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <sys/stat.h>

#include "bracket_pair.h"

void free_all(char **program, struct bracket_pair *brackets,
                    unsigned char *array, struct location **breakpoints);

int check_array_size(char *array_size_string);

char **getlines(char *filename);

struct location *make_location(size_t i, size_t j);

void putescchar(unsigned char c);

#endif /* UTILS_H */
