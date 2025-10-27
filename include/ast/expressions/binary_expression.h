#ifndef BINARY_EXPRESSION_H
#define BINARY_EXPRESSION_H

#include "lib/errors.h"
#include "ast/expressions/expression.h"

typedef enum {
    BINARY_EXPRESSION_TYPE_ADDITION,
    BINARY_EXPRESSION_TYPE_SUBTRACTION,
    BINARY_EXPRESSION_TYPE_MULTIPLICATION,
    BINARY_EXPRESSION_TYPE_DIVISION,
    BINARY_EXPRESSION_TYPE_MODULO
} BinaryExpressionType;

typedef struct {
    BinaryExpressionType type;
    Expression* left;
    Expression* right;
} BinaryExpression;

ZencError binary_expression_new(BinaryExpressionType type, Expression* left, Expression* right, BinaryExpression** expression);
void binary_expression_free(BinaryExpression* expression);

#endif