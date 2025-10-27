#ifndef TOKENS_H
#define TOKENS_H

#include "lib/errors.h"
#include <stdlib.h>

typedef enum {
    TOKEN_TYPE_INVALID,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_LINEBREAK
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    size_t length;
    size_t line;
    size_t pos;
} Token;

ZencError token_new(TokenType type, const char* value, size_t line, size_t pos, Token** token);
ZencError token_copy(const Token* token, Token** copy);
void token_free(Token* token);

#endif