#include "ast/statements/definition.h"
#include "helpers/assertions.h"
#include <stdlib.h>
#include <string.h>

ZencError definition_new(const char* variable, Definition** definition) 
{
    ASSERT_NOT_NULL(variable);
    ASSERT_NOT_NULL(definition);

    Definition* d = (Definition*)malloc(sizeof(Definition));
    ASSERT_ALLOC(d);

    char* var = strdup(variable);
    ASSERT_ALLOC_FREE(var, d);

    d->variable = var;

    *definition = d;
    return ZENC_ERROR_OK;
}

void definition_free(Definition* definition)
{
    RETURN_IF_NULL(definition);

    free(definition->variable);
    free(definition);
}