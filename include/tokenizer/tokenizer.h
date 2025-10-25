#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "lib/errors.h"
#include "tokenizer/tokens.h"
#include "collections/typed_list.h"
#include "collections/typed_list_iterator.h"
#include <stdlib.h>

DECLARE_TYPED_LIST(Token, token);
DECLARE_TYPED_ITERATOR(Token, token);

typedef struct {
    char* token;
    int line;
    int pos;
} TokenizerError;

DECLARE_TYPED_LIST(TokenizerError, tokenizer_error);
DECLARE_TYPED_ITERATOR(TokenizerError, tokenizer_error);

typedef struct {
    char* input;
    size_t input_length;
    size_t pos;

    TokenList* token_list;
    TokenizerErrorList* error_list;
} Tokenizer;

ZencError tokenizer_new(char* input, Tokenizer** tokenizer);
void tokenizer_free(Tokenizer* tokenizer);

ZencError tokenizer_tokenize(Tokenizer* tokenizer);
TokenList* tokenizer_get_token_list(Tokenizer* tokenizer);
bool tokenizer_had_error(Tokenizer* tokenizer);
TokenizerErrorList* tokenizer_get_errors(Tokenizer* tokenizer);

#endif