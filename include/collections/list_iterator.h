#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H

#include "collections/linked_list.h"
#include <stdbool.h>

typedef struct {
    Node* current;
} ListIterator;

ZencError list_iterator_init(ListIterator* iterator, const LinkedList* list);
bool list_iterator_has_next(const ListIterator* iterator);
const void* list_iterator_next(ListIterator* iterator);

#endif