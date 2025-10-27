#include "parser/parser.h"
#include "lang/keywords.h"
#include "helpers/assertions.h"
#include <stdlib.h>
#include <string.h>

typedef bool (*TokenPredicate)(const Token*);

static bool token_predicate_is_linebreak(const Token* token) { return token->type == TOKEN_TYPE_LINEBREAK; }
static bool token_predicate_is_no_linebreak(const Token* token) { return !token_predicate_is_linebreak(token); }

static void advance_linebreaks(Parser* parser);
static void advance_until_linebreak(Parser* parser);
static void advance_until_match(Parser* parser, TokenPredicate predicate);
static const Token* advance(Parser* parser);

static ZencError expect_next_token(Parser* parser, bool* result);
static ZencError expect_token_type(Parser* parser, TokenType type, bool* result);
static ZencError expect_keyword(Parser* parser, const char* keword, bool* result);
static ZencError expect_linebreak_or_end(Parser* parser, bool* result);

static ZencError parse_statement(Parser* parser, Statement** statement, bool* success);
static ZencError parse_declaration(Parser* parser, Declaration** declaration, bool* success);
static ZencError parse_print_statement(Parser* parser, PrintStatement** print_statement, bool* success);
static ZencError parse_expression(Parser* parser, Expression** expression, bool* success);
static ZencError parse_literal(Parser* parser, Literal** literal, bool* success);
static ZencError parse_variable(Parser* parser, char** variable, bool* success);

static ZencError handle_unexpected_token_error(Parser* parser);
static ZencError handle_missing_token_error(Parser* parser);
static ZencError handle_error(Parser* parser, ParserError* error);

ZencError parser_new(const TokenList* tokens, Parser** parser)
{
    ASSERT_NOT_NULL(tokens);
    ASSERT_NOT_NULL(parser);

    Parser* p = (Parser*)calloc(1, sizeof(Parser));
    ASSERT_ALLOC(p);

    ZencError err = token_list_iterator_init(&p->token_iterator, tokens);
    ASSERT_NO_ERROR_FREE(err, p);

    err = statement_list_new(&p->statements);
    ASSERT_NO_ERROR_FREE(err, p);

    err = parser_error_list_new(&p->errors);
    if (IS_ERROR(err))
    {
        statement_list_free(p->statements);
        free(p);
        return err;
    }

    *parser = p;
    return ZENC_ERROR_OK;
}

ZencError parser_parse(Parser* parser)
{
    ASSERT_NOT_NULL(parser);

    while(token_list_iterator_has_next(&parser->token_iterator))
    {
        advance_linebreaks(parser);
        if (!token_list_iterator_has_next(&parser->token_iterator))
            break;
        
        bool success = false;
        Statement* statement = NULL;
        ZencError err = parse_statement(parser, &statement, &success);
        ASSERT_NO_ERROR(err);

        if (success)
        {
            err = statement_list_append(parser->statements, statement);
            if (IS_ERROR(err))
            {
                statement_free(statement);
                return err;
            }
        }

        err = expect_linebreak_or_end(parser, &success);
        ASSERT_NO_ERROR(err);
    }

    return ZENC_ERROR_OK;
}

const StatementList* parser_get_statements(const Parser* parser)
{
    if (!parser) return NULL;
    return parser->statements;
}

bool parser_had_error(const Parser* parser)
{
    if (!parser) return false;
    return !parser_error_list_is_empty(parser->errors);
}

const ParserErrorList* parser_get_errors(const Parser* parser)
{
    if (!parser) return NULL;
    return parser->errors;
}

void parser_free(Parser* parser)
{
    RETURN_IF_NULL(parser);

    statement_list_free(parser->statements);
    free(parser);
}

static ZencError parse_statement(Parser* parser, Statement** statement, bool* success)
{
    *success = false;
    *statement = NULL;

    ZencError err = expect_token_type(parser, TOKEN_TYPE_KEYWORD, success);
    if (IS_ERROR(err) || !*success) return err;

    StatementType statement_type;
    Declaration* declaration = NULL;
    PrintStatement* print_statement = NULL;
    const Token* token = token_list_iterator_peek(&parser->token_iterator);

    if (strcmp(token->value, KEYWORD_INHALE) == 0)
    {
        err = parse_declaration(parser, &declaration, success);
        statement_type = STATEMENT_TYPE_DECLARATION;
    }
    else if (strcmp(token->value, KEYWORD_EXHALE) == 0)
    {
        err = parse_print_statement(parser, &print_statement, success);
        statement_type = STATEMENT_TYPE_PRINT;
    }
    else
    {
        return handle_unexpected_token_error(parser);
    }

    if (IS_ERROR(err) || !*success) return err;
    
    switch (statement_type)
    {
        case STATEMENT_TYPE_DECLARATION: err = statement_declaration_new(declaration, statement); break;
        case STATEMENT_TYPE_PRINT: err = statement_print_new(print_statement, statement); break;
    }

    if (IS_ERROR(err))
    {
        *success = false;
        declaration_free(declaration);
        print_statement_free(print_statement);
        return err;
    }

    return ZENC_ERROR_OK;
}

static ZencError parse_declaration(Parser* parser, Declaration** declaration, bool* success)
{
    *success = false;
    *declaration = NULL;

    ZencError err;
    Expression* expression = NULL;
    char* variable = NULL;

    err = expect_keyword(parser, KEYWORD_INHALE, success);
    if (IS_ERROR(err) || !*success) goto fail;
    (void)advance(parser);
    
    err = parse_expression(parser, &expression, success);
    if (IS_ERROR(err) || !*success) goto fail;
    
    err = expect_keyword(parser, KEYWORD_AS, success);
    if (IS_ERROR(err) || !*success) goto fail;
    (void)advance(parser);
    
    err = parse_variable(parser, &variable, success);
    if (IS_ERROR(err) || !*success) goto fail;

    Declaration* dec = NULL;
    err = declaration_new(variable, expression, &dec);
    if (IS_ERROR(err)) goto fail;

    free(variable);
    *declaration = dec;
    return ZENC_ERROR_OK;

fail:
    *success = false;
    expression_free(expression);
    free(variable);
    return err;
}

static ZencError parse_print_statement(Parser* parser, PrintStatement** print_statement, bool* success)
{
    *success = false;
    *print_statement = NULL;

    ZencError err;
    Expression* expression = NULL;

    err = expect_keyword(parser, KEYWORD_EXHALE, success);
    if (IS_ERROR(err) || !*success) goto fail;
    (void)advance(parser);

    err = parse_expression(parser, &expression, success);
    if (IS_ERROR(err) || !*success) goto fail;

    PrintStatement* statement = NULL;
    err = print_statement_new(expression, &statement);
    if (IS_ERROR(err)) goto fail;

    *print_statement = statement;
    return ZENC_ERROR_OK;

fail:
    *success = false;
    expression_free(expression);
    return err;
}

static ZencError parse_expression(Parser* parser, Expression** expression, bool* success)
{
    *success = false;
    *expression = NULL;

    ZencError err;
    ExpressionType type;
    Literal* literal = NULL;
    char* variable = NULL;

    err = expect_next_token(parser, success);
    if (IS_ERROR(err) || !*success) return err;

    const Token* token = token_list_iterator_peek(&parser->token_iterator);
    switch (token->type)
    {
        case TOKEN_TYPE_IDENTIFIER:
            type = EXPRESSION_TYPE_VARIABLE;
            err = parse_variable(parser, &variable, success);
            break;
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_STRING:
            type = EXPRESSION_TYPE_LITERAL;
            err = parse_literal(parser, &literal, success);
            break;
        default:
            *success = false;
            err = handle_unexpected_token_error(parser);
            break;
    }

    if (IS_ERROR(err) || !*success) return err;
    
    switch (type)
    {
        case EXPRESSION_TYPE_VARIABLE: err = expression_variable_new(variable, expression); break;
        case EXPRESSION_TYPE_LITERAL: err = expression_literal_new(literal, expression); break;
    }

    if (IS_ERROR(err))
    {
        *success = false;
        literal_free(literal);
        free(variable);
        return err;
    }

    free(variable);
    return ZENC_ERROR_OK;
}

static ZencError parse_literal(Parser* parser, Literal** literal, bool* success)
{
    *success = false;
    *literal = NULL;

    ZencError err = expect_next_token(parser, success);
    if (IS_ERROR(err) || !*success) return err;

    LiteralType type;
    const Token* token = token_list_iterator_peek(&parser->token_iterator);
    switch (token->type)
    {
        case TOKEN_TYPE_NUMBER: type = LITERAL_TYPE_NUMBER; break;
        case TOKEN_TYPE_STRING: type = LITERAL_TYPE_STRING; break;
        default:
            *success = false;
            return handle_unexpected_token_error(parser);
    }

    (void)advance(parser);
    err = literal_new(type, token->value, literal);
    if (IS_ERROR(err)) return err;

    return ZENC_ERROR_OK;
}

static ZencError parse_variable(Parser* parser, char** variable, bool* success)
{
    *success = false;
    *variable = NULL;

    ZencError err = expect_token_type(parser, TOKEN_TYPE_IDENTIFIER, success);
    if (IS_ERROR(err) || !*success) return err;

    const Token* token = token_list_iterator_peek(&parser->token_iterator);
    char* var = strdup(token->value);
    ASSERT_ALLOC(var);

    (void)advance(parser);

    *variable = var;
    *success = true;
    return ZENC_ERROR_OK;
}

static void advance_linebreaks(Parser* parser)
{
    advance_until_match(parser, token_predicate_is_no_linebreak);
}

static void advance_until_linebreak(Parser* parser)
{
    advance_until_match(parser, token_predicate_is_linebreak);
}

static void advance_until_match(Parser* parser, TokenPredicate predicate)
{
    while(token_list_iterator_has_next(&parser->token_iterator))
    {
        const Token* token = token_list_iterator_peek(&parser->token_iterator);
        if (predicate(token))
            break;
        
        (void)advance(parser);
    }
}

static const Token* advance(Parser* parser)
{
    parser->previous_token = token_list_iterator_peek(&parser->token_iterator);
    return token_list_iterator_next(&parser->token_iterator);
}

static ZencError expect_next_token(Parser* parser, bool* result)
{
    *result = token_list_iterator_has_next(&parser->token_iterator);
    if (!*result)
        return handle_missing_token_error(parser);
    return ZENC_ERROR_OK;
}

static ZencError expect_token_type(Parser* parser, TokenType type, bool* result)
{
    ZencError err = expect_next_token(parser, result);
    if (IS_ERROR(err) || !*result) return err;

    const Token* token = token_list_iterator_peek(&parser->token_iterator);
    *result = token->type == type;
    if (!*result)
        return handle_unexpected_token_error(parser);
    return ZENC_ERROR_OK;
}

static ZencError expect_keyword(Parser* parser, const char* keyword, bool* result)
{
    ZencError err = expect_token_type(parser, TOKEN_TYPE_KEYWORD, result);
    if (IS_ERROR(err) || !*result) return err;

    const Token* token = token_list_iterator_peek(&parser->token_iterator);
    *result = (strcmp(token->value, keyword) == 0);
    if (!*result)
        return handle_unexpected_token_error(parser);
    return ZENC_ERROR_OK;
}

static ZencError expect_linebreak_or_end(Parser* parser, bool* result)
{
    if (!token_list_iterator_has_next(&parser->token_iterator))
    {
        *result = true;
        return ZENC_ERROR_OK;
    }

    return expect_token_type(parser, TOKEN_TYPE_LINEBREAK, result);
}

static ZencError handle_unexpected_token_error(Parser* parser)
{
    ParserError* err = NULL;
    const Token* error_token = token_list_iterator_peek(&parser->token_iterator);
    ZencError e = parser_error_unexpected_token_new(error_token, &err);
    ASSERT_NO_ERROR(e);
    return handle_error(parser, err);
}

static ZencError handle_missing_token_error(Parser* parser)
{
    if (!parser->previous_token)
        return ZENC_ERROR_INTERNAL;

    ParserError* err = NULL;
    ZencError e = parser_error_missing_token_new(parser->previous_token, &err);
    ASSERT_NO_ERROR(e);
    return handle_error(parser, err);
}

static ZencError handle_error(Parser* parser, ParserError* error)
{
    ZencError e = parser_error_list_append(parser->errors, error);
    if (IS_ERROR(e))
    {
        parser_error_free(error);
        return e;
    }

    advance_until_linebreak(parser);
    return ZENC_ERROR_OK;
}