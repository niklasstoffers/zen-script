#include "tokenizer/tokenizer.h"
#include "lang/keywords.h"
#include "tokenizer/token_utils.h"
#include "helpers/assertions.h"
#include <string.h>
#include <stdbool.h>

static ZencError tokenizer_next(Tokenizer* tokenizer, Token** token);
static TokenType get_token_type(const char* token);
static ZencError add_invalid_token_error(Tokenizer* tokenizer, const Token* token);

ZencError tokenizer_new(const char* input, Tokenizer** tokenizer)
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
    t->token_list = token_list;
    t->error_list = error_list;
    t->line = 1;
    t->line_pos = 1;
    
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
                err = add_invalid_token_error(tokenizer, token);
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


const TokenList* tokenizer_get_token_list(const Tokenizer* tokenizer)
{
    if (!tokenizer) return NULL;
    return tokenizer->token_list;
}

bool tokenizer_had_error(const Tokenizer* tokenizer)
{
    if (!tokenizer) return false;
    return !tokenizer_error_list_is_empty(tokenizer->error_list);
}

const TokenizerErrorList* tokenizer_get_errors(const Tokenizer* tokenizer)
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
    size_t token_offset = next_token - tokenizer->input;
    size_t token_line_pos = tokenizer->line_pos + (token_offset - tokenizer->pos);
    size_t new_tokenizer_pos = token_end - tokenizer->input;
    char *token_value = strndup(next_token, token_length);
    ASSERT_ALLOC(token_value);

    Token* t = NULL;
    ZencError err = token_new(get_token_type(token_value), token_value, tokenizer->line, token_line_pos, &t);
    ASSERT_NO_ERROR_FREE(err, token_value);
    free(token_value);

    if (t->type == TOKEN_TYPE_LINEBREAK)
    {
        tokenizer->line++;
        tokenizer->line_pos = 1;
    }
    else
    {
        tokenizer->line_pos += new_tokenizer_pos - tokenizer->pos;
    }
    tokenizer->pos = new_tokenizer_pos;
    
    *token = t;
    return ZENC_ERROR_OK;
}

static TokenType get_token_type(const char* token)
{
    if (token_is_keyword(token)) return TOKEN_TYPE_KEYWORD;
    else if (token_is_identifier(token)) return TOKEN_TYPE_IDENTIFIER;
    else if (token_is_number(token)) return TOKEN_TYPE_NUMBER;
    else if (token_is_string(token)) return TOKEN_TYPE_STRING;
    else if (token_is_linebreak(token)) return TOKEN_TYPE_LINEBREAK;
    else if (token_is_plus(token)) return TOKEN_TYPE_PLUS;
    else if (token_is_minus(token)) return TOKEN_TYPE_MINUS;
    else if (token_is_asterisk(token)) return TOKEN_TYPE_ASTERISK;
    else if (token_is_slash(token)) return TOKEN_TYPE_SLASH;
    else if (token_is_percent(token)) return TOKEN_TYPE_PERCENT;
    return TOKEN_TYPE_INVALID;
}

static ZencError add_invalid_token_error(Tokenizer* tokenizer, const Token* token)
{
    TokenizerError* error = NULL;
    ZencError err = tokenizer_error_new(token->value, token->line, token->pos, &error);
    ASSERT_NO_ERROR(err);

    err = tokenizer_error_list_append(tokenizer->error_list, error);
    if (IS_ERROR(err))
    {
        tokenizer_error_free(error);
        return err;
    }

    return ZENC_ERROR_OK;
}
