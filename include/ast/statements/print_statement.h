#ifndef PRINT_STATEMENT_H
#define PRINT_STATEMENT_H

#include "lib/errors.h"
#include "ast/expressions/expression.h"

typedef struct {
    Expression* expression;
} PrintStatement;

ZencError print_statement_new(Expression* expression, PrintStatement** statement);
void print_statement_free(PrintStatement* statement);

#endif