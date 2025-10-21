#include "tokenizer.h"
#include "keywords.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static size_t get_next_token_pos(const Tokenizer* tokenizer);
static size_t get_token_end(const Tokenizer* tokenizer);

static TokenType get_token_type(const char* token);
static bool is_keyword(const char* token);
static bool is_identifier(const char* token);
static bool is_number(const char* token);

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

    tokenizer->pos = get_next_token_pos(tokenizer);
    if (tokenizer->pos >= tokenizer->input_length)
        return NULL;

    size_t token_end = get_token_end(tokenizer);
    size_t token_length = token_end - tokenizer->pos;
    char *token_start = tokenizer->input + tokenizer->pos;
    char *token_value = strndup(token_start, token_length);

    tokenizer->pos = token_end;

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

static size_t get_next_token_pos(const Tokenizer* tokenizer)
{
    size_t pos = tokenizer->pos;
    while(pos < tokenizer->input_length) {
        char c = tokenizer->input[pos];
        if (!isspace(c))
            break;
        pos++;
    }

    return pos;
}

static size_t get_token_end(const Tokenizer* tokenizer)
{
    size_t pos = tokenizer->pos;
    while(pos < tokenizer->input_length) {
        char c = tokenizer->input[pos];
        if (isspace(c))
            break;
        pos++;
    }
    
    return pos;
}

static TokenType get_token_type(const char* token)
{
    if (is_keyword(token)) return TOKEN_TYPE_KEYWORD;
    else if (is_identifier(token)) return TOKEN_TYPE_IDENTIFIER;
    else if (is_number(token)) return TOKEN_TYPE_NUMBER;
    return TOKEN_TYPE_INVALID;
}

static bool is_keyword(const char* token)
{
    return (strcmp(token, KEYWORD_INHALE) == 0)
        || (strcmp(token, KEYWORD_EXHALE) == 0)
        || (strcmp(token, KEYWORD_AS) == 0);
}

static bool is_identifier(const char* token)
{
    if (!*token) return false;
    while (*token) {
        if (!isalpha((unsigned char)*token)) return false;
        token++;
    }
    return true;
}

static bool is_number(const char* token)
{
    if (!*token) return false;
    while (*token) {
        if (!isdigit((unsigned char)*token)) return false;
        token++;
    }
    return true;
}