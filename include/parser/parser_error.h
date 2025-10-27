#ifndef PARSER_ERROR_H
#define PARSER_ERROR_H

#include "lib/errors.h"
#include "tokenizer/tokens.h"

typedef enum {
    PARSER_ERROR_UNEXPECTED_TOKEN,
    PARSER_ERROR_MISSING_TOKEN
} ParserErrorType;

typedef struct {
    Token* token;
} ParserErrorUnexpectedToken;

typedef struct {
    Token* previous_token;
} ParserErrorMissingToken;

typedef struct {
    ParserErrorType type;
    union {
        ParserErrorUnexpectedToken* unexpected_token;
        ParserErrorMissingToken* missing_token;
    };
} ParserError;

ZencError parser_error_unexpected_token_new(const Token* token, ParserError** error);
ZencError parser_error_missing_token_new(const Token* previous_token, ParserError** error);
void parser_error_free(ParserError* error);

#endif