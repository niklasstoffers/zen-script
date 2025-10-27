#ifndef STATEMENT_H
#define STATEMENT_H

#include "lib/errors.h"
#include "ast/statements/declaration.h"
#include "ast/statements/definition.h"
#include "ast/statements/assignment.h"
#include "ast/statements/print_statement.h"

typedef enum {
    STATEMENT_TYPE_DECLARATION,
    STATEMENT_TYPE_DEFINITION,
    STATEMENT_TYPE_ASSIGNMENT,
    STATEMENT_TYPE_PRINT
} StatementType;

typedef struct {
    StatementType type;
    union {
        Declaration* declaration;
        Definition* definition;
        Assignment* assignment;
        PrintStatement* print_statement;
    };
} Statement;

ZencError statement_declaration_new(Declaration* declaration, Statement** statement);
ZencError statement_print_new(PrintStatement* print_statement, Statement** statement);
ZencError statement_definition_new(Definition* definition, Statement** statement);
ZencError statement_assignment_new(Assignment* assignment, Statement** statement);
void statement_free(Statement* statement);

#endif