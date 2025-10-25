#include "tokenizer/tokens.h"
#include "helpers/assertions.h"

void token_free(Token* token)
{
    RETURN_IF_NULL(token);

    free(token->value);
    free(token);
}