#ifndef UNARY_EXPRESSION_H
#define UNARY_EXPRESSION_H

#include "lib/errors.h"

typedef struct Expression Expression;

typedef enum {
    UNARY_EXPRESSION_TYPE_NEGATION
} UnaryExpressionType;

typedef struct {
    UnaryExpressionType type;
    Expression* expression;
} UnaryExpression;

ZencError unary_expression_new(UnaryExpressionType type, Expression* expression, UnaryExpression** unary_expression);
void unary_expression_free(UnaryExpression* unary_expression);

#endif