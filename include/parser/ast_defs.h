// #ifndef AST_DEFS_H
// #define AST_DEFS_H

// typedef enum {
//     EXPRESSION_TYPE_NUMBER,
//     EXPRESSION_TYPE_IDENTIFIER
// } ExpressionType;

// typedef enum {
//     STATEMENT_TYPE_DECLARATION,
//     STATEMENT_TYPE_PRINT
// } StatementType;

// typedef struct {
//     ExpressionType type;
//     union {
//         int number;
//         char* identifier;
//     };
// } Expression;

// typedef struct {
//     char* identifier;
//     Expression* expression;
// } Declaration;

// typedef struct {
//     Expression* expression;
// } PrintStatement;

// typedef struct {
//     StatementType type;
//     union {
//         Declaration* declaration;
//         PrintStatement* print_statement;
//     };
// } Statement;

// typedef struct StatementNode {
//     Statement* statement;
//     struct StatementNode* next;
// } StatementNode;

// typedef struct {
//     StatementNode* head;
// } Program;

// void free_program(Program* program);
// void free_statement(Statement* statement);
// void free_print_statement(PrintStatement* print_statement);
// void free_declaration(Declaration* declaration);
// void free_expression(Expression* expression);

// #endif