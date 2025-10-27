#include "ast/expressions/binary_expression.h"
#include "ast/expressions/expression.h"
#include "helpers/assertions.h"
#include <stdlib.h>

ZencError binary_expression_new(BinaryExpressionType type, Expression* left, Expression* right, BinaryExpression** expression)
{
    ASSERT_NOT_NULL(left);
    ASSERT_NOT_NULL(right);
    ASSERT_NOT_NULL(expression);

    BinaryExpression* exp = (BinaryExpression*)malloc(sizeof(BinaryExpression));
    ASSERT_ALLOC(exp);

    exp->left = left;
    exp->right = right;
    exp->type = type;

    *expression = exp;
    return ZENC_ERROR_OK;
}

void binary_expression_free(BinaryExpression* expression)
{
    RETURN_IF_NULL(expression);

    expression_free(expression->left);
    expression_free(expression->right);
    free(expression);
}