#include "ast/statements/statement.h"
#include "helpers/assertions.h"
#include <stdlib.h>

ZencError statement_declaration_new(Declaration* declaration, Statement** statement)
{
    ASSERT_NOT_NULL(declaration);
    ASSERT_NOT_NULL(statement);

    Statement* s = (Statement*)calloc(1, sizeof(Statement));
    ASSERT_ALLOC(s);

    s->type = STATEMENT_TYPE_DECLARATION;
    s->declaration = declaration;

    *statement = s;
    return ZENC_ERROR_OK;
}

ZencError statement_print_new(PrintStatement* print_statement, Statement** statement)
{
    ASSERT_NOT_NULL(print_statement);
    ASSERT_NOT_NULL(statement);

    Statement* s = (Statement*)calloc(1, sizeof(Statement));
    ASSERT_ALLOC(s);

    s->type = STATEMENT_TYPE_PRINT;
    s->print_statement = print_statement;

    *statement = s;
    return ZENC_ERROR_OK;
}

void statement_free(Statement* statement)
{
    RETURN_IF_NULL(statement);

    switch(statement->type)
    {
        case STATEMENT_TYPE_DECLARATION: declaration_free(statement->declaration); break;
        case STATEMENT_TYPE_PRINT: print_statement_free(statement->print_statement); break;
    }

    free(statement);
}