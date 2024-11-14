#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <sys/types.h>

struct list
{
    ssize_t data;
    struct list *next;
};

struct list *list_add(struct list *l, ssize_t e);

struct list *list_find(struct list *l, ssize_t e);

struct list *list_remove(struct list *l, ssize_t e);

void list_destroy(struct list *l);

#endif /* !LIST_H */
