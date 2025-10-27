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

ZencError statement_definition_new(Definition* definition, Statement** statement)
{
    ASSERT_NOT_NULL(definition);
    ASSERT_NOT_NULL(statement);

    Statement* s = (Statement*)calloc(1, sizeof(Statement));
    ASSERT_ALLOC(s);

    s->type = STATEMENT_TYPE_DEFINITION;
    s->definition = definition;

    *statement = s;
    return ZENC_ERROR_OK;
}

ZencError statement_assignment_new(Assignment* assignment, Statement** statement)
{
    ASSERT_NOT_NULL(assignment);
    ASSERT_NOT_NULL(statement);

    Statement* s = (Statement*)calloc(1, sizeof(Statement));
    ASSERT_ALLOC(s);

    s->type = STATEMENT_TYPE_ASSIGNMENT;
    s->assignment = assignment;

    *statement = s;
    return ZENC_ERROR_OK;
}

void statement_free(Statement* statement)
{
    RETURN_IF_NULL(statement);

    switch(statement->type)
    {
        case STATEMENT_TYPE_DECLARATION: declaration_free(statement->declaration); break;
        case STATEMENT_TYPE_DEFINITION: definition_free(statement->definition); break;
        case STATEMENT_TYPE_ASSIGNMENT: assignment_free(statement->assignment); break;
        case STATEMENT_TYPE_PRINT: print_statement_free(statement->print_statement); break;
    }

    free(statement);
}