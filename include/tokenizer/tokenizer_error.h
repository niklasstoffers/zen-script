#ifndef TOKENIZER_ERROR_H
#define TOKENIZER_ERROR_H

typedef struct {
    char* token;
    int line;
    int pos;
} TokenizerError;

void tokenizer_error_free(TokenizerError* error);

#endif