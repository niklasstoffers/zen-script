#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "lib/errors.h"
#include "ast/expressions/expression.h"

typedef struct {
    char* variable;
    Expression* expression;
} Assignment;

ZencError assignment_new(const char* variable, Expression* expression, Assignment** assignment);
void assignment_free(Assignment* assignment);

#endif