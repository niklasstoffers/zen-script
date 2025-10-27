#include "collections/list_iterator.h"
#include "helpers/assertions.h"

ZencError list_iterator_init(ListIterator* iterator, const LinkedList* list)
{
    ASSERT_NOT_NULL(iterator);
    ASSERT_NOT_NULL(list);
    
    iterator->current = list->head;
    return ZENC_ERROR_OK;
}

bool list_iterator_has_next(const ListIterator* iterator)
{
    if (!iterator) return false;
    return iterator->current != NULL;
}

const void* list_iterator_next(ListIterator* iterator)
{
    if (!iterator) return NULL;
    if (iterator->current)
    {
        void* current = iterator->current->data;
        iterator->current = iterator->current->next;
        return current;
    }
    return NULL;
}

const void* list_iterator_peek(const ListIterator* iterator)
{
    if (!iterator) return NULL;
    if (!iterator->current) return NULL;
    return iterator->current->data;
}