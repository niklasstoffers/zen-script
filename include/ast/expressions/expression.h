#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "lib/errors.h"
#include "ast/expressions/literal.h"
#include "ast/expressions/binary_expression.h"
#include "ast/expressions/unary_expression.h"

typedef enum {
    EXPRESSION_TYPE_LITERAL,
    EXPRESSION_TYPE_VARIABLE,
    EXPRESSION_TYPE_UNARY,
    EXPRESSION_TYPE_BINARY
} ExpressionType;

typedef struct {
    ExpressionType type;
    union {
        Literal* literal;
        char* variable;
        UnaryExpression* unary_expression;
        BinaryExpression* binary_expression;
    };
} Expression;

ZencError expression_literal_new(Literal* literal, Expression** expression);
ZencError expression_variable_new(const char* variable, Expression** expression);
ZencError expression_unary_new(UnaryExpression* unary_expression, Expression** expression);
ZencError expression_binary_new(BinaryExpression* binary_expression, Expression** expression);
void expression_free(Expression* expression);

#endif