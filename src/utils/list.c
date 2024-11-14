#include "list.h"

struct list *list_add(struct list *l, ssize_t e)
{
    struct list *new_list = malloc(sizeof(struct list));
    if (new_list)
    {
        new_list->next = l;
        new_list->data = e;
        return new_list;
    }
    return l;
}

struct list *list_find(struct list *l, ssize_t e)
{
    if (!l)
        return NULL;
    if (l->data == e)
        return l;
    return list_find(l->next, e);
}

struct list *list_remove(struct list *l, ssize_t e)
{
    if (!l)
        return NULL;

    if (l->data == e)
    {
        struct list *queue = l->next;
        free(l);
        return queue;
    }
    l->next = list_remove(l->next, e);
    return l;
}

void list_destroy(struct list *l)
{
    if (!l)
        return;

    list_destroy(l->next);
    free(l);
}
