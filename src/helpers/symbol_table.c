#include "helpers/symbol_table.h"
#include <string.h>

static size_t hash(const char* str, size_t table_size);

ZencError symbol_table_new(size_t num_buckets, SymbolTable** table)
{
    if (!table)
        return ZENC_ERROR_INVALID_ARG;
    
    SymbolTable* t = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!t)
        return ZENC_ERROR_NOMEM;

    t->table_size = num_buckets;
    t->buckets = (SymbolNode**)calloc(num_buckets, sizeof(SymbolNode*));
    if (!t->buckets) 
    {
        free(t);
        return ZENC_ERROR_NOMEM;
    }

    *table = t;
    return ZENC_ERROR_OK;
}

ZencError symbol_exists(SymbolTable* table, const char* symbol, bool* exists)
{
    if (!table || !symbol || !exists)
        return ZENC_ERROR_INVALID_ARG;
    
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

ZencError symbol_add(SymbolTable* table, const char* symbol)
{
    if (!table || !symbol)
        return ZENC_ERROR_INVALID_ARG;

    SymbolNode* new_node = (SymbolNode*)malloc(sizeof(SymbolNode));
    if (!new_node)
        return ZENC_ERROR_NOMEM;

    new_node->symbol = strdup(symbol);
    if (!new_node->symbol) 
    {
        free(new_node);
        return ZENC_ERROR_NOMEM;
    }
    
    size_t h = hash(symbol, table->table_size);
    SymbolNode* node = table->buckets[h];
    SymbolNode* prev = NULL;
    while(node) 
    {
        if (strcmp(node->symbol, symbol) == 0)
        {
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
    if (!table)
        return;

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