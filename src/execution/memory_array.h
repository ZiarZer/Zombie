#ifndef MEMORY_ARRAY_H
#define MEMORY_ARRAY_H

#define DEFAULT_ARRAY_SIZE 100

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct memory_array {
    unsigned char *data;
    ssize_t size;
    ssize_t max_size;
    ssize_t cursor;
};

struct memory_array *array_init(ssize_t array_size);

char array_get_current(struct memory_array *a);

void array_set_current(struct memory_array *a, char c);

char array_get_at(struct memory_array *a, ssize_t index);

void array_set_at(struct memory_array *a, ssize_t index, char c);

int array_move(struct memory_array *a, ssize_t new_index);

void change_array_size(struct memory_array *a, ssize_t new_size);

void destroy_array(struct memory_array *a);

#endif /* !MEMORY_ARRAY_H */
