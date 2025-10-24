#include "tokenizer/tokenizer.h"
#include "lang/keywords.h"
#include "tokenizer/token_utils.h"
#include <string.h>
#include <stdbool.h>

static TokenType get_token_type(const char* token);
static ZencError tokenizer_error_invalid_token(const char* token, TokenizerError** error);

static void tokenizer_error_invalid_token_free(TokenizerErrorInvalidToken* error);
static void token_node_free(TokenNode* node);

ZencError tokenizer_new(char* input, Tokenizer** tokenizer)
{
    if (!input || !tokenizer)
        return ZENC_ERROR_INVALID_ARG;

    Tokenizer* t = (Tokenizer*)malloc(sizeof(Tokenizer));
    if (!t)
        return ZENC_ERROR_NOMEM;

    t->input = strdup(input);
    if (!t->input) 
    {
        free(t);
        return ZENC_ERROR_NOMEM;
    }
        
    t->input_length = strlen(t->input);
    t->pos = 0;
    
    *tokenizer = t;
    return ZENC_ERROR_OK;
}

void tokenizer_free(Tokenizer* tokenizer)
{
    if (!tokenizer)
        return;

    free(tokenizer->input);
    free(tokenizer);
}

ZencError tokenizer_next(Tokenizer* tokenizer, Token** token)
{
    if (!tokenizer || !token)
        return ZENC_ERROR_INVALID_ARG;

    *token = NULL;
    char* next_token = (char*)find_next_token(tokenizer->input + tokenizer->pos);
    if (!next_token)
        return ZENC_ERROR_OK;

    char* token_end = (char*)find_token_end(next_token);
    size_t token_length = token_end - next_token;
    size_t new_tokenizer_pos = token_end - tokenizer->input;
    char *token_value = strndup(next_token, token_length);

    if (!token_value)
        return ZENC_ERROR_NOMEM;

    Token* t = (Token*)malloc(sizeof(Token));
    if (!t)
    {
        free(token_value);
        return ZENC_ERROR_NOMEM;
    }

    t->value = token_value;
    t->length = token_length;
    t->type = get_token_type(token_value);

    tokenizer->pos = new_tokenizer_pos;
    
    *token = t;
    return ZENC_ERROR_OK;
}

ZencError tokenizer_tokenize(Tokenizer* tokenizer, TokenStream** stream, TokenizerError** error)
{
    if (!tokenizer || !stream || !error)
        return ZENC_ERROR_INVALID_ARG;

    *error = NULL;
    TokenStream* token_stream = (TokenStream*)malloc(sizeof(TokenStream));
    if (!token_stream)
        return ZENC_ERROR_NOMEM;

    TokenNode* current = NULL;
    while (true) 
    {
        Token* token = NULL;
        ZencError err = tokenizer_next(tokenizer, &token);
        if (err != ZENC_ERROR_OK) 
        {
            free(token_stream);
            return err;
        }

        if (token)
        {
            if (token->type == TOKEN_TYPE_INVALID)
            {
                token_stream_free(token_stream);
                err = tokenizer_error_invalid_token(token->value, error);
                token_free(token);
                return err;
            }
        }
        else
        {
            break;
        }

        TokenNode* next = (TokenNode*)malloc(sizeof(TokenNode));
        if (!next) 
        {
            token_free(token);
            token_stream_free(token_stream);
            return ZENC_ERROR_NOMEM;
        }

        next->token = token;
        if (!current)
            token_stream->head = next;
        else
            current->next = next;
        current = next;
    }

    *stream = token_stream;
    return ZENC_ERROR_OK;
}

void tokenizer_error_free(TokenizerError* error)
{
    if (!error)
        return;

    tokenizer_error_invalid_token_free(error->invalid_token_error);
    free(error);
}

void token_stream_free(TokenStream* stream)
{
    if (!stream)
        return;

    token_node_free(stream->head);
    free(stream);
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

static ZencError tokenizer_error_invalid_token(const char* token, TokenizerError** error)
{
    TokenizerError* tokenizer_error = (TokenizerError*)malloc(sizeof(TokenizerError));
    TokenizerErrorInvalidToken* invalid_token_error = (TokenizerErrorInvalidToken*)malloc(sizeof(TokenizerErrorInvalidToken));
    char* token_value = strdup(token);

    if (!tokenizer_error || !invalid_token_error || !token_value)
    {
        free(tokenizer_error);
        free(invalid_token_error);
        free(token_value);
        return ZENC_ERROR_NOMEM;
    }

    invalid_token_error->token = token_value;
    invalid_token_error->line = 0;
    invalid_token_error->pos = 0;
    tokenizer_error->type = TOKENIZER_ERROR_INVALID_TOKEN;
    tokenizer_error->invalid_token_error = invalid_token_error;

    *error = tokenizer_error;
    return ZENC_ERROR_OK;
}

static void tokenizer_error_invalid_token_free(TokenizerErrorInvalidToken* error)
{
    if (!error)
        return;
    
    free(error->token);
    free(error);
}

static void token_node_free(TokenNode* node)
{
    if (!node)
        return;
    
    token_node_free(node->next);
    token_free(node->token);
    free(node);
}