#include "ast/statements/print_statement.h"
#include "helpers/assertions.h"
#include <stdlib.h>

ZencError print_statement_new(Expression* expression, PrintStatement** statement)
{
    ASSERT_NOT_NULL(expression);
    ASSERT_NOT_NULL(statement);

    PrintStatement* s = (PrintStatement*)malloc(sizeof(PrintStatement));
    ASSERT_ALLOC(s);

    s->expression = expression;

    *statement = s;
    return ZENC_ERROR_OK;
}

void print_statement_free(PrintStatement* statement)
{
    RETURN_IF_NULL(statement);

    expression_free(statement->expression);
    free(statement);
}