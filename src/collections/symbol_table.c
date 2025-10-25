#include "collections/symbol_table.h"
#include "helpers/assertions.h"
#include <string.h>

static size_t hash(const char* str, size_t table_size);

ZencError symbol_table_new(size_t num_buckets, SymbolTable** table)
{
    ASSERT_NOT_NULL(table);
    ASSERT_GREATER_ZERO(num_buckets);
    
    SymbolTable* t = (SymbolTable*)malloc(sizeof(SymbolTable));
    ASSERT_ALLOC(t);

    t->table_size = num_buckets;
    t->buckets = (SymbolNode**)calloc(num_buckets, sizeof(SymbolNode*));
    ASSERT_ALLOC_FREE(t->buckets, t);

    *table = t;
    return ZENC_ERROR_OK;
}

ZencError symbol_table_exists(const SymbolTable* table, const char* symbol, bool* exists)
{
    ASSERT_NOT_NULL(table);
    ASSERT_NOT_NULL(symbol);
    ASSERT_NOT_NULL(exists);
    
    size_t h = hash(symbol, table->table_size);
    SymbolNode* node = table->buckets[h];
    while(node) 
    {
        if (strcmp(node->symbol, symbol) == 0)
        {
            *exists = true;
            return ZENC_ERROR_OK;
        }
        node = node->next;
    }

    *exists = false;
    return ZENC_ERROR_OK;
}

ZencError symbol_table_add(SymbolTable* table, const char* symbol)
{
    ASSERT_NOT_NULL(table);
    ASSERT_NOT_NULL(symbol);

    SymbolNode* new_node = (SymbolNode*)malloc(sizeof(SymbolNode));
    ASSERT_ALLOC(new_node);

    new_node->symbol = strdup(symbol);
    ASSERT_ALLOC_FREE(new_node->symbol, new_node);
    
    size_t h = hash(symbol, table->table_size);
    SymbolNode* node = table->buckets[h];
    SymbolNode* prev = NULL;
    while(node) 
    {
        if (strcmp(node->symbol, symbol) == 0)
        {
            free(new_node->symbol);
            free(new_node);
            return ZENC_ERROR_INVALID_ARG;
        }

        prev = node;
        node = node->next;
    }

    if (!prev)
        table->buckets[h] = new_node;
    else
        prev->next = new_node;

    return ZENC_ERROR_OK;
}

void symbol_table_free(SymbolTable* table)
{
    RETURN_IF_NULL(table);

    for(size_t i = 0; i < table->table_size; i++) 
    {
        SymbolNode* node = table->buckets[i];
        while (node)
        {
            SymbolNode* next = node->next;
            free(node->symbol);
            free(node);
            node = next;
        }
    }

    free(table->buckets);
    free(table);
}

static size_t hash(const char* str, size_t table_size)
{
    size_t h = 0;
    while (*str) h = 31 * h + (unsigned char)(*str++);
    return h % table_size;
}