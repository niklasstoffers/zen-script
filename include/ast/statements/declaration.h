#ifndef DECLARATION_H
#define DECLARATION_H

#include "lib/errors.h"
#include "ast/expressions/expression.h"

typedef struct {
    char* variable;
    Expression* expression;
} Declaration;

ZencError declaration_new(const char* variable, Expression* expression, Declaration** declaration);
void declaration_free(Declaration* declaration);

#endif