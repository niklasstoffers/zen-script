#ifndef LITERAL_H
#define LITERAL_H

#include "lib/errors.h"

typedef enum {
    LITERAL_TYPE_NUMBER,
    LITERAL_TYPE_STRING
} LiteralType;

typedef struct {
    LiteralType type;
    char* value;
} Literal;

ZencError literal_new(LiteralType type, const char* value, Literal** literal);
void literal_free(Literal* literal);

#endif