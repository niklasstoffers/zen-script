#include "ast/expressions/expression.h"
#include "helpers/assertions.h"
#include <stdlib.h>
#include <string.h>

ZencError expression_literal_new(Literal* literal, Expression** expression)
{
    ASSERT_NOT_NULL(literal);
    ASSERT_NOT_NULL(expression);

    Expression* e = (Expression*)malloc(sizeof(Expression));
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

    Expression* e = (Expression*)malloc(sizeof(Expression));
    ASSERT_ALLOC(e);

    char* var = strdup(variable);
    ASSERT_ALLOC_FREE(var, e);

    e->type = EXPRESSION_TYPE_VARIABLE;
    e->variable = var;

    *expression = e;
    return ZENC_ERROR_OK;
}

ZencError expression_unary_new(UnaryExpression* unary_expression, Expression** expression)
{
    ASSERT_NOT_NULL(unary_expression);
    ASSERT_NOT_NULL(expression);

    Expression* e = (Expression*)malloc(sizeof(Expression));
    ASSERT_ALLOC(e);

    e->type = EXPRESSION_TYPE_UNARY;
    e->unary_expression = unary_expression;

    *expression = e;
    return ZENC_ERROR_OK;
}

ZencError expression_binary_new(BinaryExpression* binary_expression, Expression** expression)
{
    ASSERT_NOT_NULL(binary_expression);
    ASSERT_NOT_NULL(expression);

    Expression* e = (Expression*)malloc(sizeof(Expression));
    ASSERT_ALLOC(e);

    e->type = EXPRESSION_TYPE_BINARY;
    e->binary_expression = binary_expression;

    *expression = e;
    return ZENC_ERROR_OK;
}

void expression_free(Expression* expression)
{
    RETURN_IF_NULL(expression);

    switch (expression->type)
    {
        case EXPRESSION_TYPE_LITERAL: literal_free(expression->literal); break;
        case EXPRESSION_TYPE_VARIABLE: free(expression->variable); break;
        case EXPRESSION_TYPE_UNARY: unary_expression_free(expression->unary_expression); break;
        case EXPRESSION_TYPE_BINARY: binary_expression_free(expression->binary_expression); break;
    }

    free(expression);
}