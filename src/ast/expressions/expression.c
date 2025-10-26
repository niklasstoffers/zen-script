#include "ast/expressions/expression.h"
#include "helpers/assertions.h"
#include <stdlib.h>
#include <string.h>

ZencError expression_literal_new(Literal* literal, Expression** expression)
{
    ASSERT_NOT_NULL(literal);
    ASSERT_NOT_NULL(expression);

    Expression* e = (Expression*)calloc(1, sizeof(Expression));
    ASSERT_ALLOC(e);

    e->type = EXPRESSION_TYPE_LITERAL;
    e->literal = literal;

    *expression = e;
    return ZENC_ERROR_OK;
}

ZencError expression_variable_new(const char* variable, Expression** expression)
{
    ASSERT_NOT_NULL(variable);
    ASSERT_NOT_NULL(expression);

    Expression* e = (Expression*)calloc(1, sizeof(Expression));
    ASSERT_ALLOC(e);

    char* var = strdup(variable);
    ASSERT_ALLOC_FREE(var, e);

    e->type = EXPRESSION_TYPE_VARIABLE;
    e->variable = var;

    *expression = e;
    return ZENC_ERROR_OK;
}

void expression_free(Expression* expression)
{
    RETURN_IF_NULL(expression);

    switch (expression->type)
    {
        EXPRESSION_TYPE_LITERAL: literal_free(expression->literal); break;
        EXPRESSION_TYPE_VARIABLE: free(expression->variable); break;
    }

    free(expression);
}