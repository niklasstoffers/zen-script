#include "tokenizer/tokenizer.h"
#include "lang/keywords.h"
#include "tokenizer/token_utils.h"
#include <string.h>
#include <stdbool.h>

static TokenType get_token_type(const char* token);

Tokenizer* tokenizer_new(char* input)
{
    if (!input)
        return NULL;

    Tokenizer* tokenizer = (Tokenizer*)malloc(sizeof(Tokenizer));

    tokenizer->input = strdup(input);
    tokenizer->input_length = strlen(tokenizer->input);
    tokenizer->pos = 0;

    return tokenizer;
}

void tokenizer_free(Tokenizer* tokenizer)
{
    if (!tokenizer)
        return;

    free(tokenizer->input);
    free(tokenizer);
}

Token* tokenizer_next(Tokenizer* tokenizer)
{
    if (!tokenizer)
        return NULL;

    char* next_token = (char*)find_next_token(tokenizer->input + tokenizer->pos);
    if (!next_token)
        return NULL;

    char* token_end = (char*)find_token_end(next_token);
    size_t token_length = token_end - next_token;
    size_t new_tokenizer_pos = token_end - tokenizer->input;
    char *token_value = strndup(next_token, token_length);

    tokenizer->pos = new_tokenizer_pos;

    Token* token = (Token*)malloc(sizeof(Token));
    token->value = token_value;
    token->length = token_length;
    token->type = get_token_type(token_value);
    return token;
}

void token_free(Token* token)
{
    if (!token)
        return;

    free(token->value);
    free(token);
}

static TokenType get_token_type(const char* token)
{
    if (token_is_keyword(token)) return TOKEN_TYPE_KEYWORD;
    else if (token_is_identifier(token)) return TOKEN_TYPE_IDENTIFIER;
    else if (token_is_number(token)) return TOKEN_TYPE_NUMBER;
    else if (token_is_linebreak(token)) return TOKEN_TYPE_LINEBREAK;
    return TOKEN_TYPE_INVALID;
}