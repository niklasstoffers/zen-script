#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "lib/errors.h"
#include "ast/expressions/literal.h"

typedef enum {
    EXPRESSION_TYPE_LITERAL,
    EXPRESSION_TYPE_VARIABLE
} ExpressionType;

typedef struct {
    ExpressionType type;
    union {
        Literal* literal;
        char* variable;
    };
} Expression;

ZencError expression_literal_new(Literal* literal, Expression** expression);
ZencError expression_variable_new(const char* variable, Expression** expression);
void expression_free(Expression* expression);

#endif