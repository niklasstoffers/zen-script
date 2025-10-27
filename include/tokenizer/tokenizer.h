#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "lib/errors.h"
#include "tokenizer/tokens.h"
#include "tokenizer/token_list.h"
#include "tokenizer/tokenizer_error_list.h"
#include <stdlib.h>

typedef struct {
    char* input;
    size_t input_length;
    size_t pos;
    size_t line;
    size_t line_pos;

    TokenList* token_list;
    TokenizerErrorList* error_list;
} Tokenizer;

ZencError tokenizer_new(const char* input, Tokenizer** tokenizer);
void tokenizer_free(Tokenizer* tokenizer);

ZencError tokenizer_tokenize(Tokenizer* tokenizer);
const TokenList* tokenizer_get_token_list(const Tokenizer* tokenizer);
bool tokenizer_had_error(const Tokenizer* tokenizer);
const TokenizerErrorList* tokenizer_get_errors(const Tokenizer* tokenizer);

#endif