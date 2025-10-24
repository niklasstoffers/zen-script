#ifndef SYBMOL_TABLE_H
#define SYMBOL_TABLE_H

#include "lib/errors.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct SymbolNode {
    char* symbol;
    struct SymbolNode* next;
} SymbolNode;

typedef struct {
    SymbolNode** buckets;
    size_t table_size;
} SymbolTable;

ZencError symbol_table_new(size_t num_buckets, SymbolTable** table);
void symbol_table_free(SymbolTable* table);

ZencError symbol_exists(SymbolTable* table, const char* symbol, bool* exists);
ZencError symbol_add(SymbolTable* table, const char* symbol);

#endif