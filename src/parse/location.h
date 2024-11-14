#ifndef LOCATION_H
#define LOCATION_H

#include <stdio.h>
#include <sys/types.h>

struct location
{
    char *filename;
    ssize_t i;
    ssize_t j;
};

struct location make_location(char *filename, size_t i, size_t j);

void print_location(struct location loc, FILE *stream);

#endif /* !LOCATION_H */
