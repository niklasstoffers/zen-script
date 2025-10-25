#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "lib/errors.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    size_t length;
} LinkedList;

ZencError linked_list_new(LinkedList** list);
ZencError linked_list_append(LinkedList* list, void* data);
bool linked_list_is_empty(const LinkedList* list);
void linked_list_free(LinkedList* list, void (*free_data)(void *));

#endif