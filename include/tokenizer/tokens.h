#ifndef TOKENS_H
#define TOKENS_H

#include <stdlib.h>

typedef enum {
    TOKEN_TYPE_INVALID,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_LINEBREAK
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    size_t length;
} Token;

void token_free(Token* token);

#endif