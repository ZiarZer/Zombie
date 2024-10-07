#ifndef ERROR_H
#define ERROR_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct location
{
    char *filename;
    size_t i;
    size_t j;
};

struct char_coords
{
    ssize_t i;
    ssize_t j;
};

void print_location(struct location loc, FILE *stream);

void print_runtime_error(char **program, char *filename,
                         struct char_coords coordinates, int error_code);
void print_error(char *line, size_t i, size_t j);

#endif /* ERROR_H */
