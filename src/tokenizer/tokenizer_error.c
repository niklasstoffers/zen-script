#include "tokenizer/tokenizer_error.h"
#include "helpers/assertions.h"
#include <string.h>

ZencError tokenizer_error_new(const char* token, size_t line, size_t pos, TokenizerError** error)
{
    ASSERT_NOT_NULL(token);
    ASSERT_NOT_NULL(error);

    TokenizerError* err = (TokenizerError*)malloc(sizeof(TokenizerError));
    ASSERT_ALLOC(err);

    char* t = strdup(token);
    ASSERT_ALLOC_FREE(t, err);

    err->token = t;
    err->line = line;
    err->pos = pos;

    *error = err;
    return ZENC_ERROR_OK;
}

void tokenizer_error_free(TokenizerError* error)
{
    RETURN_IF_NULL(error);

    free(error->token);
    free(error);
}