#ifndef TOKENIZER_H
#define TOKENIZER_H

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

typedef struct {
    char* input;
    size_t input_length;
    size_t pos;
} Tokenizer;

Tokenizer* tokenizer_new(char* input);
void tokenizer_free(Tokenizer* tokenizer);
Token* tokenizer_next(Tokenizer* tokenizer);
void token_free(Token* token);

#endif