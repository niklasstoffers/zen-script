#include "tokenizer/tokenizer.h"
#include "lang/keywords.h"
#include "tokenizer/token_utils.h"
#include "helpers/assertions.h"
#include <string.h>
#include <stdbool.h>

static ZencError tokenizer_next(Tokenizer* tokenizer, Token** token);
static TokenType get_token_type(const char* token);
static ZencError add_invalid_token_error(Tokenizer* tokenizer, const char* token);

ZencError tokenizer_new(char* input, Tokenizer** tokenizer)
{
    ASSERT_NOT_NULL(input);
    ASSERT_NOT_NULL(tokenizer);

    Tokenizer* t = (Tokenizer*)calloc(1, sizeof(Tokenizer));
    ASSERT_ALLOC(t);

    ZencError err = ZENC_ERROR_OK;
    char* tokenizer_input = strdup(input);
    TokenList* token_list = NULL;
    TokenizerErrorList* error_list = NULL;

    err = token_list_new(&token_list);
    if (IS_ERROR(err)) goto fail;
    err = tokenizer_error_list_new(&error_list);
    if (IS_ERROR(err)) goto fail;


    t->input = tokenizer_input;
    t->input_length = strlen(t->input);
    t->pos = 0;
    t->token_list = token_list;
    t->error_list = error_list;
    
    *tokenizer = t;
    return ZENC_ERROR_OK;

fail:
    free(t);
    free(tokenizer_input);
    token_list_free(token_list);
    tokenizer_error_list_free(error_list);
    return err;
}

ZencError tokenizer_tokenize(Tokenizer* tokenizer)
{
    ASSERT_NOT_NULL(tokenizer);

    ZencError err = ZENC_ERROR_OK;
    Token* token = NULL;
    do {
        err = tokenizer_next(tokenizer, &token);
        ASSERT_NO_ERROR(err);

        if (token)
        {
            if (token->type == TOKEN_TYPE_INVALID)
            {
                err = add_invalid_token_error(tokenizer, token->value);
                if (IS_ERROR(err)) goto fail;
            }

            err = token_list_append(tokenizer->token_list, token);
            if (IS_ERROR(err)) goto fail;
        }
    } while(token != NULL);

    return ZENC_ERROR_OK;

fail:
    token_free(token);
    return err;
}


TokenList* tokenizer_get_token_list(Tokenizer* tokenizer)
{
    if (!tokenizer) return NULL;
    return tokenizer->token_list;
}

bool tokenizer_had_error(Tokenizer* tokenizer)
{
    if (!tokenizer) return false;
    return !tokenizer_error_list_is_empty(tokenizer->error_list);
}

TokenizerErrorList* tokenizer_get_errors(Tokenizer* tokenizer)
{
    if (!tokenizer) return NULL;
    return tokenizer->error_list;
}

void tokenizer_free(Tokenizer* tokenizer)
{
    RETURN_IF_NULL(tokenizer);

    free(tokenizer->input);
    token_list_free(tokenizer->token_list);
    tokenizer_error_list_free(tokenizer->error_list);
    free(tokenizer);
}

static ZencError tokenizer_next(Tokenizer* tokenizer, Token** token)
{
    *token = NULL;
    char* next_token = (char*)find_next_token(tokenizer->input + tokenizer->pos);
    if (!next_token)
        return ZENC_ERROR_OK;

    char* token_end = (char*)find_token_end(next_token);
    size_t token_length = token_end - next_token;
    size_t new_tokenizer_pos = token_end - tokenizer->input;
    char *token_value = strndup(next_token, token_length);
    ASSERT_ALLOC(token_value);

    Token* t = (Token*)malloc(sizeof(Token));
    ASSERT_ALLOC_FREE(t, token_value);

    t->value = token_value;
    t->length = token_length;
    t->type = get_token_type(token_value);

    tokenizer->pos = new_tokenizer_pos;
    
    *token = t;
    return ZENC_ERROR_OK;
}

static TokenType get_token_type(const char* token)
{
    if (token_is_keyword(token)) return TOKEN_TYPE_KEYWORD;
    else if (token_is_identifier(token)) return TOKEN_TYPE_IDENTIFIER;
    else if (token_is_number(token)) return TOKEN_TYPE_NUMBER;
    else if (token_is_linebreak(token)) return TOKEN_TYPE_LINEBREAK;
    return TOKEN_TYPE_INVALID;
}

static ZencError add_invalid_token_error(Tokenizer* tokenizer, const char* token)
{
    TokenizerError* error = (TokenizerError*)malloc(sizeof(TokenizerError));
    ASSERT_ALLOC(error);

    char* token_value = strdup(token);
    ASSERT_ALLOC_FREE(token_value, error);

    error->token = token_value;
    error->line = 0;
    error->pos = 0;

    ZencError err = tokenizer_error_list_append(tokenizer->error_list, error);
    if (IS_ERROR(err))
    {
        tokenizer_error_free(error);
        return err;
    }

    return ZENC_ERROR_OK;
}
