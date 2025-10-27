#ifndef TYPED_LIST_ITERATOR_H
#define TYPED_LIST_ITERATOR_H

#include "collections/list_iterator.h"

#define DECLARE_TYPED_ITERATOR(TYPE, FUNC_PREFIX) \
    typedef ListIterator TYPE##ListIterator; \
    ZencError FUNC_PREFIX##_list_iterator_init(TYPE##ListIterator* iterator, const TYPE##List* list); \
    bool FUNC_PREFIX##_list_iterator_has_next(const TYPE##ListIterator* iterator); \
    const TYPE* FUNC_PREFIX##_list_iterator_next(TYPE##ListIterator* iterator); \
    const TYPE* FUNC_PREFIX##_list_iterator_peek(const TYPE##ListIterator* iterator);

#define DEFINE_TYPED_ITERATOR(TYPE, FUNC_PREFIX) \
    ZencError FUNC_PREFIX##_list_iterator_init(TYPE##ListIterator* iterator, const TYPE##List* list) \
    { \
        return list_iterator_init((ListIterator*)iterator, (LinkedList*)list); \
    } \
    bool FUNC_PREFIX##_list_iterator_has_next(const TYPE##ListIterator* iterator) \
    { \
        return list_iterator_has_next((ListIterator*)iterator); \
    } \
    const TYPE* FUNC_PREFIX##_list_iterator_next(TYPE##ListIterator* iterator) \
    { \
        return (TYPE*)list_iterator_next((ListIterator*)iterator); \
    } \
    const TYPE* FUNC_PREFIX##_list_iterator_peek(const TYPE##ListIterator* iterator) \
    { \
        return (TYPE*)list_iterator_peek((ListIterator*)iterator); \
    }

#endif