#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "lib/errors.h"
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

typedef struct TokenNode {
    Token* token;
    struct TokenNode* next;
} TokenNode;

typedef struct {
    TokenNode* head;
} TokenStream;

typedef enum {
    TOKENIZER_ERROR_OK,
    TOKENIZER_ERROR_INVALID_TOKEN
} TokenizerErrorType;

typedef struct {
    char* token;
    int line;
    int pos;
} TokenizerErrorInvalidToken;

typedef struct {
    TokenizerErrorType type;
    union {
        TokenizerErrorInvalidToken* invalid_token_error;
    };
} TokenizerError;

typedef struct {
    char* input;
    size_t input_length;
    size_t pos;
} Tokenizer;

ZencError tokenizer_new(char* input, Tokenizer** tokenizer);
void tokenizer_free(Tokenizer* tokenizer);
ZencError tokenizer_next(Tokenizer* tokenizer, Token** token);
ZencError tokenizer_tokenize(Tokenizer* tokenizer, TokenStream** stream, TokenizerError** error);
void tokenizer_error_free(TokenizerError* error);
void token_stream_free(TokenStream* stream);
void token_free(Token* token);

#endif