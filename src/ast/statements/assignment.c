#include "ast/statements/assignment.h"
#include "helpers/assertions.h"
#include <stdlib.h>
#include <string.h>

ZencError assignment_new(const char* variable, Expression* expression, Assignment** assignment)
{
    ASSERT_NOT_NULL(variable);
    ASSERT_NOT_NULL(expression);
    ASSERT_NOT_NULL(assignment);

    Assignment* a = (Assignment*)malloc(sizeof(Assignment));
    ASSERT_ALLOC(a);

    char* var = strdup(variable);
    ASSERT_ALLOC_FREE(var, a);

    a->variable = var;
    a->expression = expression;

    *assignment = a;
    return ZENC_ERROR_OK;
}

void assignment_free(Assignment* assignment)
{
    RETURN_IF_NULL(assignment);

    free(assignment->variable);
    expression_free(assignment->expression);
    free(assignment);
}