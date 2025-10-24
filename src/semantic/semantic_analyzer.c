#include "semantic/semantic_analyzer.h"
#include "helpers/symbol_table.h"
#include <string.h>

static ZencError validate_declaration(Declaration* declaration, SymbolTable* symbol_table, SemanticErrorList* error_list, SemanticErrorNode** current_error);
static ZencError validate_expression(Expression* expression, SymbolTable* symbol_table, SemanticErrorList* error_list, SemanticErrorNode** current_error);

static ZencError add_undeclared_identifier_error(SemanticErrorList* error_list, SemanticErrorNode** current, char* identifier);
static ZencError add_redeclaration_error(SemanticErrorList* error_list, SemanticErrorNode** current, char* identifier);
static ZencError add_semantic_error(SemanticErrorList* error_list, SemanticErrorNode** current, SemanticErrorType type, void* error);

static void free_semantic_error_node(SemanticErrorNode* node);
static void free_semantic_error(SemanticError* error);

ZencError validate_semantic(Program* program, SemanticErrorList** errors)
{
    if (!program || !errors)
        return ZENC_ERROR_INVALID_ARG;

    SymbolTable* symbol_table = NULL;
    ZencError err = symbol_table_new(SYMBOL_TABLE_SIZE, &symbol_table);
    if (err != ZENC_ERROR_OK)
        return err;

    SemanticErrorList* error_list = (SemanticErrorList*)calloc(1, sizeof(SemanticErrorList));
    SemanticErrorNode* current_error = NULL;
    if (!error_list)
    {
        symbol_table_free(symbol_table);
        return ZENC_ERROR_NOMEM;
    }

    StatementNode* current = program->head;
    while (current)
    {
        switch(current->statement->type)
        {
            case STATEMENT_TYPE_DECLARATION: err = validate_declaration(current->statement->declaration, symbol_table, error_list, &current_error); break;
            case STATEMENT_TYPE_PRINT: err = validate_expression(current->statement->print_statement->expression, symbol_table, error_list, &current_error); break;
            default: err = ZENC_ERROR_INTERNAL; break;
        }
        
        if (err != ZENC_ERROR_OK)
        {
            symbol_table_free(symbol_table);
            free_semantic_error_list(error_list);
            return err;
        }

        current = current->next;
    }

    *errors = error_list;
    return ZENC_ERROR_OK;
}

static ZencError validate_declaration(Declaration* declaration, SymbolTable* symbol_table, SemanticErrorList* error_list, SemanticErrorNode** current_error)
{
    ZencError err = validate_expression(declaration->expression, symbol_table, error_list, current_error);
    if (err != ZENC_ERROR_OK)
        return err;
    
    bool valid_identifier = false;
    (void)symbol_exists(symbol_table, declaration->identifier, &valid_identifier);
    if (valid_identifier)
        return add_redeclaration_error(error_list, current_error, declaration->identifier);
    
    err = symbol_add(symbol_table, declaration->identifier);
    return err;
}

static ZencError validate_expression(Expression* expression, SymbolTable* symbol_table, SemanticErrorList* error_list, SemanticErrorNode** current_error)
{
    if (expression->type == EXPRESSION_TYPE_IDENTIFIER)
    {
        bool valid_identifier = false;
        (void)symbol_exists(symbol_table, expression->identifier, &valid_identifier);
        if (!valid_identifier)
            return add_undeclared_identifier_error(error_list, current_error, expression->identifier);
    }
    return ZENC_ERROR_OK;
}

static ZencError add_undeclared_identifier_error(SemanticErrorList* error_list, SemanticErrorNode** current, char* identifier)
{
    SemanticErrorUndeclaredIdentifier* undeclared_error = (SemanticErrorUndeclaredIdentifier*)malloc(sizeof(SemanticErrorUndeclaredIdentifier));
    char* undeclared_identifier = strdup(identifier);

    if (!undeclared_error || !undeclared_identifier)
    {
        free(undeclared_error);
        free(undeclared_identifier);
        return ZENC_ERROR_NOMEM;
    }

    undeclared_error->identifier = undeclared_identifier;
    return add_semantic_error(error_list, current, SEMANTIC_ERROR_UNDECLARED_IDENTIFIER, (void*)undeclared_error);
}

static ZencError add_redeclaration_error(SemanticErrorList* error_list, SemanticErrorNode** current, char* identifier)
{
    SemanticErrorRedeclaration* redeclaration_error = (SemanticErrorRedeclaration*)malloc(sizeof(SemanticErrorRedeclaration));
    char* redeclared_identifier = strdup(identifier);

    if (!redeclaration_error || !redeclared_identifier)
    {
        free(redeclaration_error);
        free(redeclared_identifier);
        return ZENC_ERROR_NOMEM;
    }

    redeclaration_error->identifier = redeclared_identifier;
    return add_semantic_error(error_list, current, SEMANTIC_ERROR_REDECLARATION, (void*)redeclaration_error);
}

static ZencError add_semantic_error(SemanticErrorList* error_list, SemanticErrorNode** current, SemanticErrorType type, void* error)
{
    SemanticError* semantic_error = (SemanticError*)malloc(sizeof(SemanticError));
    SemanticErrorNode* error_node = (SemanticErrorNode*)malloc(sizeof(SemanticErrorNode));

    if (!semantic_error || !error_node)
    {
        free(semantic_error);
        free(error_node);
        return ZENC_ERROR_NOMEM;
    }

    semantic_error->type = type;
    switch (type) 
    {
        case SEMANTIC_ERROR_UNDECLARED_IDENTIFIER: semantic_error->undeclared_identifier = (SemanticErrorUndeclaredIdentifier*)error; break;
        case SEMANTIC_ERROR_REDECLARATION: semantic_error->redeclaration = (SemanticErrorRedeclaration*)error; break;
        default:
            free(semantic_error);
            free(error_node);
            return ZENC_ERROR_INVALID_ARG;
    }

    error_node->error = semantic_error;
    if (!*current)
        error_list->head = error_node;
    else
        (*current)->next = error_node;

    *current = error_node;
    return ZENC_ERROR_OK;
}

void free_semantic_error_list(SemanticErrorList* errors)
{
    if (!errors)
        return;

    free_semantic_error_node(errors->head);
}

static void free_semantic_error_node(SemanticErrorNode* node)
{
    if (!node)
        return;

    free_semantic_error_node(node->next);
    free_semantic_error(node->error);
    free(node);
}

static void free_semantic_error(SemanticError* error)
{
    if (!error)
        return;

    if (error->type == SEMANTIC_ERROR_REDECLARATION)
    {
        free(error->redeclaration->identifier);
        free(error->redeclaration);
    }
    else if (error->type == SEMANTIC_ERROR_UNDECLARED_IDENTIFIER)
    {
        free(error->undeclared_identifier->identifier);
        free(error->undeclared_identifier);
    }

    free(error);
}