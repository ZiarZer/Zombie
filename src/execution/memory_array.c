#include "memory_array.h"

struct memory_array *array_init(ssize_t array_size) {
    size_t init_size = array_size > 0 ? array_size : DEFAULT_ARRAY_SIZE;

    struct memory_array *a = malloc(sizeof(struct memory_array));
    a->cursor = 0;
    a->max_size = array_size;
    a->size = init_size;
    a->data = calloc(init_size, sizeof(char));

    return a;
}

char array_get_current(struct memory_array *a) {
    return array_get_at(a, a->cursor);
}

void array_set_current(struct memory_array *a, char c) {
    array_set_at(a, a->cursor, c);
}

char array_get_at(struct memory_array *a, ssize_t index) {
    return a->data[index];
}

void array_set_at(struct memory_array *a, ssize_t index, char c) {
    a->data[index] = c;
}

/**
 * \brief Move array cursor and reallocate array if necessary.
 * \param a the memory array.
 * \param new_index the new position of the cursor.
 * \return 0 if success, 1 if the new position is too big and the array's size is fixed.
 */
int array_move(struct memory_array *a, ssize_t new_index) {
    if (new_index >= a->size) {
        if (a->max_size != 0)
            return 1;
        size_t new_size = new_index > a->size + 10 ? new_index : a->size + 10;
        change_array_size(a, new_size);
    }
    a->cursor = new_index;
    return 0;
}

void change_array_size(struct memory_array *a, ssize_t new_size) {
    ssize_t old_size = a->size;
    a->size = new_size;
    a->data = realloc(a->data, new_size * sizeof(unsigned char));
    if (new_size > old_size) {
        memset(a->data + old_size, 0, new_size - old_size);
    }
}

void destroy_array(struct memory_array *a) {
    if (!a)
        return;
    free(a->data);
    free(a);
}
