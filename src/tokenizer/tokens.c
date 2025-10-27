#include "tokenizer/tokens.h"
#include "helpers/assertions.h"
#include <string.h>

ZencError token_new(TokenType type, const char* value, size_t line, size_t pos, Token** token)
{
    ASSERT_NOT_NULL(value);
    ASSERT_NOT_NULL(token);

    Token* t = (Token*)malloc(sizeof(Token));
    ASSERT_ALLOC(t);

    char* val = strdup(value);
    ASSERT_ALLOC_FREE(val, t);

    t->type = type;
    t->value = val;
    t->length = strlen(t->value);
    t->line = line;
    t->pos = pos;

    *token = t;
    return ZENC_ERROR_OK;
}

ZencError token_copy(const Token* token, Token** copy)
{
    ASSERT_NOT_NULL(token);
    ASSERT_NOT_NULL(copy);

    return token_new(token->type, token->value, token->line, token->pos, copy);
}

void token_free(Token* token)
{
    RETURN_IF_NULL(token);

    free(token->value);
    free(token);
}