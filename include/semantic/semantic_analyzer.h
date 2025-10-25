// #ifndef SEMANTIC_ANALYZER_H
// #define SEMANTIC_ANALYZER_H

// #include "lib/errors.h"
// #include "parser/ast_defs.h"

// #define SYMBOL_TABLE_SIZE 1024U

// typedef enum {
//     SEMANTIC_ERROR_REDECLARATION,
//     SEMANTIC_ERROR_UNDECLARED_IDENTIFIER
// } SemanticErrorType;

// typedef struct {
//     char* identifier;
// } SemanticErrorRedeclaration;

// typedef struct {
//     char* identifier;
// } SemanticErrorUndeclaredIdentifier;

// typedef struct {
//     SemanticErrorType type;
//     union {
//         SemanticErrorRedeclaration* redeclaration;
//         SemanticErrorUndeclaredIdentifier* undeclared_identifier;
//     };
// } SemanticError;

// typedef struct SemanticErrorNode {
//     SemanticError* error;
//     struct SemanticErrorNode* next;
// } SemanticErrorNode;

// typedef struct {
//     SemanticErrorNode* head;
// } SemanticErrorList;

// ZencError validate_semantic(Program* program, SemanticErrorList** errors);
// void free_semantic_error_list(SemanticErrorList* errors);

// #endif