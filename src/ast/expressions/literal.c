#include "ast/expressions/literal.h"
#include "helpers/assertions.h"
#include <stdlib.h>
#include <string.h>

ZencError literal_new(LiteralType type, const char* value, Literal** literal)
{
    ASSERT_NOT_NULL(value);
    ASSERT_NOT_NULL(literal);

    Literal* l = (Literal*)malloc(sizeof(Literal));
    ASSERT_ALLOC(l);

    char* val = strdup(value);
    ASSERT_ALLOC_FREE(val, l);

    l->type = type;
    l->value = val;

    *literal = l;
    return ZENC_ERROR_OK;
}

void literal_free(Literal* literal)
{
    RETURN_IF_NULL(literal);

    free(literal->value);
    free(literal);
}