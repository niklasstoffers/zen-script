#ifndef TOKENIZER_ERROR_H
#define TOKENIZER_ERROR_H

#include "lib/errors.h"
#include <stdlib.h>

typedef struct {
    char* token;
    size_t line;
    size_t pos;
} TokenizerError;

ZencError tokenizer_error_new(const char* token, size_t line, size_t pos, TokenizerError** error);
void tokenizer_error_free(TokenizerError* error);

#endif