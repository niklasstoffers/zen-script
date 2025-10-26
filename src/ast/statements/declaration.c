#include "ast/statements/declaration.h"
#include "helpers/assertions.h"
#include <stdlib.h>
#include <string.h>

ZencError declaration_new(const char* variable, Expression* expression, Declaration** declaration) 
{
    ASSERT_NOT_NULL(variable);
    ASSERT_NOT_NULL(expression);
    ASSERT_NOT_NULL(declaration);

    Declaration* d = (Declaration*)malloc(sizeof(Declaration));
    ASSERT_ALLOC(d);

    char* var = strdup(variable);
    ASSERT_ALLOC_FREE(var, d);

    d->variable = var;
    d->expression = expression;

    *declaration = d;
    return ZENC_ERROR_OK;
}

void declaration_free(Declaration* declaration)
{
    RETURN_IF_NULL(declaration);

    free(declaration->variable);
    expression_free(declaration->expression);
    free(declaration);
}