#include "tokenizer/tokenizer_error.h"
#include "helpers/assertions.h"

void tokenizer_error_free(TokenizerError* error)
{
    RETURN_IF_NULL(error);

    free(error->token);
    free(error);
}