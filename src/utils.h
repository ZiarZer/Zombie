#ifndef UTILS_H
#define UTILS_H

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <sys/stat.h>

#include "bracket_pair.h"

int free_and_return(char **program, struct bracket_pair *brackets,
                    unsigned char *array, int return_value);

int check_array_size(char *array_size_string);

char **getlines(char *filename);

#endif /* UTILS_H */
