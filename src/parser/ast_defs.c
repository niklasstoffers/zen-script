// #include "parser/ast_defs.h"
// #include <stdlib.h>

// static void free_statement_node(StatementNode* node);

// void free_program(Program* program)
// {
//     if (!program)
//         return;

//     free_statement_node(program->head);
// }

// void free_statement(Statement* statement)
// {
//     if (!statement)
//         return;

//     if (statement->type == STATEMENT_TYPE_DECLARATION)
//         free_declaration(statement->declaration);
//     else if (statement->type == STATEMENT_TYPE_PRINT)
//         free_print_statement(statement->print_statement);

//     free(statement);
// }

// void free_print_statement(PrintStatement* print_statement)
// {
//     if (!print_statement)
//         return;

//     free_expression(print_statement->expression);
//     free(print_statement);
// }

// void free_declaration(Declaration* declaration)
// {
//     if (!declaration)
//         return;

//     free_expression(declaration->expression);
//     free(declaration->identifier);
//     free(declaration);
// }

// void free_expression(Expression* expression)
// {
//     if (!expression)
//         return;

//     if (expression->type == EXPRESSION_TYPE_IDENTIFIER)
//         free(expression->identifier);
//     free(expression);
// }

// static void free_statement_node(StatementNode* node)
// {
//     if (!node)
//         return;

//     free_statement_node(node->next);
//     free_statement(node->statement);
//     free(node);
// }