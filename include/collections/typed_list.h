#ifndef TYPED_LIST_H
#define TYPED_LIST_H

#include "collections/linked_list.h"

#define DECLARE_TYPED_LIST(TYPE, FUNC_PREFIX) \
    typedef LinkedList TYPE##List; \
    ZencError FUNC_PREFIX##_list_new(TYPE##List **list); \
    ZencError FUNC_PREFIX##_list_append(TYPE##List *list, TYPE *data); \
    bool FUNC_PREFIX##_list_is_empty(TYPE##List *list); \
    void FUNC_PREFIX##_list_free(TYPE##List *list);

#define DEFINE_TYPED_LIST(TYPE, FUNC_PREFIX, FREE_FUNC) \
    ZencError FUNC_PREFIX##_list_new(TYPE##List **list) { \
        return linked_list_new((LinkedList **)list); \
    } \
    ZencError FUNC_PREFIX##_list_append(TYPE##List *list, TYPE *data) { \
        return linked_list_append((LinkedList *)list, (void *)data); \
    } \
    bool FUNC_PREFIX##_list_is_empty(TYPE##List *list) { \
        return linked_list_is_empty((LinkedList *)list); \
    } \
    void FUNC_PREFIX##_list_free(TYPE##List *list) { \
        linked_list_free((LinkedList *)list, (void (*)(void *))FREE_FUNC); \
    }

#endif