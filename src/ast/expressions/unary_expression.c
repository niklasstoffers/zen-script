#include "ast/expressions/unary_expression.h"
#include "ast/expressions/expression.h"
#include "helpers/assertions.h"
#include <stdlib.h>

ZencError unary_expression_new(UnaryExpressionType type, Expression* expression, UnaryExpression** unary_expression)
{
    ASSERT_NOT_NULL(expression);
    ASSERT_NOT_NULL(unary_expression);

    UnaryExpression* exp = (UnaryExpression*)malloc(sizeof(UnaryExpression));
    ASSERT_ALLOC(exp);

    exp->type = type;
    exp->expression = expression;

    *unary_expression = exp;
    return ZENC_ERROR_OK;
}

void unary_expression_free(UnaryExpression* unary_expression)
{
    RETURN_IF_NULL(unary_expression);

    expression_free(unary_expression->expression);
    free(unary_expression);
}