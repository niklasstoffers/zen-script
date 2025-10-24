#ifndef PARSER_H
#define PARSER_H

#include "lib/errors.h"
#include "tokenizer/tokenizer.h"
#include "parser/ast_defs.h"

typedef enum {
    PARSER_ERROR_OK,
    PARSER_ERROR_INVALID
} ParserError;

ZencError parse_program(TokenStream* tokens, Program** program, ParserError* error);

#endif