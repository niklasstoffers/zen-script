#ifndef PARSER_H
#define PARSER_H

#include "lib/errors.h"
#include "tokenizer/token_list.h"
#include "parser/parser_error_list.h"
#include "ast/program.h"
#include <stdbool.h>

typedef struct {
    TokenListIterator token_iterator;
    StatementList* statements;
    ParserErrorList* errors;
    const Token* previous_token;
} Parser;

ZencError parser_new(const TokenList* tokens, Parser** parser);
ZencError parser_parse(Parser* parser);
const StatementList* parser_get_statements(const Parser* parser);
bool parser_had_error(const Parser* parser);
const ParserErrorList* parser_get_errors(const Parser* parser);
void parser_free(Parser* parser);

#endif