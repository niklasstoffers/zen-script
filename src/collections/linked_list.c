#include "collections/linked_list.h"
#include "helpers/assertions.h"

ZencError linked_list_new(LinkedList** list)
{
    ASSERT_NOT_NULL(list);
    
    LinkedList* l = (LinkedList*)calloc(1, sizeof(LinkedList));
    ASSERT_ALLOC(l);

    *list = l;
    return ZENC_ERROR_OK;
}

ZencError linked_list_append(LinkedList* list, void* data)
{
    ASSERT_NOT_NULL(list);
    ASSERT_NOT_NULL(data);
    
    Node* node = (Node*)calloc(1, sizeof(Node));
    ASSERT_ALLOC(node);

    node->data = data;
    if (!list->head)
        list->head = node;
    else
        list->tail->next = node;

    list->tail = node;
    list->length++;

    return ZENC_ERROR_OK;
}

bool linked_list_is_empty(LinkedList* list)
{
    if (!list) return true;
    return list->length == 0;
}

void linked_list_free(LinkedList* list, void (*free_data)(void *))
{
    RETURN_IF_NULL(list);

    Node* node = list->head;
    while(node)
    {
        Node* next = node->next;
        if (free_data)
            free_data(node->data);
        free(node);
        node = next;
    }

    free(list);
}