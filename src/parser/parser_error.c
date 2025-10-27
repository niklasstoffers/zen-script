#include "parser/parser_error.h"
#include "helpers/assertions.h"
#include <stdlib.h>
#include <string.h>

ZencError parser_error_unexpected_token_new(const Token* token, ParserError** error)
{
    ASSERT_NOT_NULL(token);
    ASSERT_NOT_NULL(error);

    ParserError* err = (ParserError*)calloc(1, sizeof(ParserError));
    ASSERT_ALLOC(err);

    ParserErrorUnexpectedToken* unexpected_token_error = (ParserErrorUnexpectedToken*)malloc(sizeof(ParserErrorUnexpectedToken));
    ASSERT_ALLOC_FREE(unexpected_token_error, err);

    Token* t = NULL;
    ZencError e = token_copy(token, &t);
    if (IS_ERROR(e))
    {
        free(err);
        free(unexpected_token_error);
        return e;
    }

    unexpected_token_error->token = t;
    err->type = PARSER_ERROR_UNEXPECTED_TOKEN;
    err->unexpected_token = unexpected_token_error;

    *error = err;
    return ZENC_ERROR_OK;
}

ZencError parser_error_missing_token_new(const Token* previous_token, ParserError** error)
{
    ASSERT_NOT_NULL(previous_token);
    ASSERT_NOT_NULL(error);

    ParserError* err = (ParserError*)calloc(1, sizeof(ParserError));
    ASSERT_ALLOC(err);

    ParserErrorMissingToken* mising_token_error = (ParserErrorMissingToken*)malloc(sizeof(ParserErrorMissingToken));
    ASSERT_ALLOC_FREE(mising_token_error, err);

    Token* t = NULL;
    ZencError e = token_copy(previous_token, &t);
    if (IS_ERROR(e))
    {
        free(err);
        free(mising_token_error);
        return e;
    }

    mising_token_error->previous_token = t;
    err->type = PARSER_ERROR_MISSING_TOKEN;
    err->missing_token = mising_token_error;

    *error = err;
    return ZENC_ERROR_OK;
}

void parser_error_free(ParserError* error)
{
    RETURN_IF_NULL(error);

    switch (error->type)
    {
        case PARSER_ERROR_UNEXPECTED_TOKEN:
            token_free(error->unexpected_token->token);
            free(error->unexpected_token);
            break;
        case PARSER_ERROR_MISSING_TOKEN:
            token_free(error->missing_token->previous_token);
            free(error->missing_token);
            break;
    }

    free(error);
}