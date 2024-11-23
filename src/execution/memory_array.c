#include "memory_array.h"

struct memory_array *array_init(ssize_t array_size) {
    size_t init_size = array_size > 0 ? array_size : DEFAULT_ARRAY_SIZE;

    struct memory_array *a = malloc(sizeof(struct memory_array));
    a->cursor = 0;
    a->low_bound = 0;
    a->high_bound = array_size > 0 ? array_size - 1 : 0;
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
    size_t real_index = index - a->low_bound;
    return a->data[real_index];
}

void array_set_at(struct memory_array *a, ssize_t index, char c) {
    size_t real_index = index - a->low_bound;
    a->data[real_index] = c;
}
/**
 * \brief Move array cursor and reallocate array if necessary.
 * \param a the memory array.
 * \param new_index the new position of the cursor.
 * \return 0 if success, 1 if the new position is too high, 2 if it is too low.
 */
int array_move(struct memory_array *a, ssize_t new_index) {
    if (new_index < a->low_bound) {
        if (a->max_size == 0) {
            change_array_size(a, a->high_bound - new_index + 1);
            memmove(a->data + a->low_bound - new_index, a->data, a->low_bound - new_index);
            memset(a->data, 0, a->low_bound - new_index);
        } else if (a->high_bound - new_index + 1 > a->max_size)
            return 2;
        a->low_bound = new_index;
    } else if (new_index > a->high_bound) {
        if (a->max_size == 0)
            change_array_size(a, new_index - a->low_bound + 1);
        else if (new_index - a->low_bound + 1 > a->max_size)
            return 1;
        a->high_bound = new_index;
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
