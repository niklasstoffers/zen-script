#include "parser/parser.h"
#include "lang/keywords.h"
#include "helpers/conversion_helper.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static ZencError parse_statement(TokenNode** tokens, Statement** statement, ParserError* error);
static ZencError parse_declaration(TokenNode** tokens, Declaration** declaration, ParserError* error);
static ZencError parse_print_statement(TokenNode** tokens, PrintStatement** print_statement, ParserError* error);
static ZencError parse_expression(TokenNode** tokens, Expression** expression, ParserError* error);

static bool expect_token(TokenNode* node, TokenType type, const char* value);
static bool expect_linebreak_or_end(TokenNode** tokens);
static void advance_linebreaks(TokenNode** tokens);

ZencError parse_program(TokenStream* tokens, Program** program, ParserError* error)
{
    if (!tokens || !program || !error)
        return ZENC_ERROR_INVALID_ARG;

    *error = PARSER_ERROR_OK;
    Program* program_node = (Program*)malloc(sizeof(Program));
    if (!program_node)
        return ZENC_ERROR_NOMEM;

    StatementNode* current_statement = NULL;
    TokenNode* current_token = tokens->head;
    while(current_token)
    {
        Statement* statement = NULL;
        ZencError err = parse_statement(&current_token, &statement, error);
        if (err != ZENC_ERROR_OK || *error != PARSER_ERROR_OK)
        {
            free_program(program_node);
            return err;
        }

        StatementNode* next_statement = (StatementNode*)malloc(sizeof(StatementNode));
        if (!next_statement)
        {
            free_statement(statement);
            free_program(program_node);
            return ZENC_ERROR_NOMEM;
        }

        next_statement->statement = statement;
        if (!current_statement)
            program_node->head = next_statement;
        else
            current_statement->next = next_statement;
        current_statement = next_statement;

        if (!expect_linebreak_or_end(&current_token))
        {
            free_program(program_node);
            *error = PARSER_ERROR_INVALID;
            return ZENC_ERROR_OK;
        }

        advance_linebreaks(&current_token);
    }

    *program = program_node;
    return ZENC_ERROR_OK;
}

static ZencError parse_statement(TokenNode** tokens, Statement** statement, ParserError* error)
{
    TokenNode* current = *tokens;
    if (current->token->type != TOKEN_TYPE_KEYWORD)
    {
        *error = PARSER_ERROR_INVALID;
        return ZENC_ERROR_OK;
    }

    Statement* statement_node = (Statement*)malloc(sizeof(Statement));
    if (!statement)
        return ZENC_ERROR_NOMEM;

    ZencError err = ZENC_ERROR_OK;
    if (strcmp(current->token->value, KEYWORD_INHALE) == 0)
    {
        current = current->next;
        Declaration* declaration = NULL;
        err = parse_declaration(&current, &declaration, error);
        statement_node->type = STATEMENT_TYPE_DECLARATION;
        statement_node->declaration = declaration;
    }
    else if (strcmp(current->token->value, KEYWORD_EXHALE) == 0)
    {
        current = current->next;
        PrintStatement* print_statement = NULL;
        err = parse_print_statement(&current, &print_statement, error);
        statement_node->type = STATEMENT_TYPE_PRINT;
        statement_node->print_statement = print_statement;
    }

    if (err != ZENC_ERROR_OK || *error != PARSER_ERROR_OK)
    {
        free_statement(statement_node);
        return err;
    }

    *statement = statement_node;
    *tokens = current;
    return ZENC_ERROR_OK;
}

static ZencError parse_declaration(TokenNode** tokens, Declaration** declaration, ParserError* error) 
{
    TokenNode* current = *tokens;
    if (!current)
    {
        *error = PARSER_ERROR_INVALID;
        return ZENC_ERROR_OK;
    }
    
    Expression* expression = NULL;
    ZencError err = parse_expression(&current, &expression, error);
    if (err != ZENC_ERROR_OK || *error != PARSER_ERROR_OK)
        return err;

    if (!expect_token(current, TOKEN_TYPE_KEYWORD, KEYWORD_AS))
        goto fail;

    current = current->next;
    if (!expect_token(current, TOKEN_TYPE_IDENTIFIER, NULL))
        goto fail;

    char* identifier = strdup(current->token->value);
    Declaration* declaration_node = (Declaration*)malloc(sizeof(Declaration));
    if (!identifier || !declaration_node)
    {
        free(identifier);
        free_declaration(declaration_node);
        free_expression(expression);
        return ZENC_ERROR_NOMEM;
    }

    current = current->next;
    declaration_node->expression = expression;
    declaration_node->identifier = identifier;
    *declaration = declaration_node;
    *tokens = current;

    return ZENC_ERROR_OK;

fail:
    free_expression(expression);
    *error = PARSER_ERROR_INVALID;
    return ZENC_ERROR_OK;
}

static ZencError parse_print_statement(TokenNode** tokens, PrintStatement** print_statement, ParserError* error)
{
    TokenNode* current = *tokens;
    if (!current)
    {
        *error = PARSER_ERROR_INVALID;
        return ZENC_ERROR_OK;
    }

    Expression* expression = NULL;
    ZencError err = parse_expression(&current, &expression, error);
    if (err != ZENC_ERROR_OK || *error != PARSER_ERROR_OK)
        return err;

    PrintStatement* print_statement_node = (PrintStatement*)malloc(sizeof(PrintStatement));
    if (!print_statement_node)
    {
        free_expression(expression);
        return ZENC_ERROR_NOMEM;
    }

    print_statement_node->expression = expression;
    *print_statement = print_statement_node;
    *tokens = current;

    return ZENC_ERROR_OK;
}

static ZencError parse_expression(TokenNode** tokens, Expression** expression, ParserError* error)
{
    TokenNode* current = *tokens;
    Expression* expression_node = (Expression*)malloc(sizeof(Expression));
    if (!expression_node)
        return ZENC_ERROR_NOMEM;

    if (expect_token(current, TOKEN_TYPE_NUMBER, NULL))
    {
        int literal_value;
        if (!string_to_int(current->token->value, &literal_value))
        {
            free_expression(expression_node);
            *error = PARSER_ERROR_INVALID;
            return ZENC_ERROR_OK;
        }

        expression_node->type = EXPRESSION_TYPE_NUMBER;
        expression_node->number = literal_value;
    }
    else if (expect_token(current, TOKEN_TYPE_IDENTIFIER, NULL))
    {
        char* identifier = strdup(current->token->value);
        if (!identifier)
        {
            free_expression(expression_node);
            return ZENC_ERROR_NOMEM;
        }

        expression_node->type = EXPRESSION_TYPE_IDENTIFIER;
        expression_node->identifier = identifier;
    }
    else
    {
        *error = PARSER_ERROR_INVALID;
        return ZENC_ERROR_OK;
    }

    current = current->next;
    *tokens = current;
    *expression = expression_node;

    return ZENC_ERROR_OK;
}

static bool expect_token(TokenNode* node, TokenType type, const char* value)
{
    if (!node) return false;
    if (node->token->type != type || (value && strcmp(node->token->value, value) != 0)) return false;
    return true;
}

static bool expect_linebreak_or_end(TokenNode** tokens)
{
    if (!*tokens)
        return true;
    return (*tokens)->token->type == TOKEN_TYPE_LINEBREAK;
}

static void advance_linebreaks(TokenNode** tokens)
{
    TokenNode* current = *tokens;
    while (current)
    {
        if (current->token->type != TOKEN_TYPE_LINEBREAK)
            break;
        current = current->next;
    }
    *tokens = current;
}
