#include "minunit/minunit.h"
#include "tokenizer/tokenizer.h"
#include <stdbool.h>
#include <string.h>

static void expect_token_sequence(const char* input, Token sequence[], size_t sequence_length, TokenizerError errors[], size_t num_errors)
{
    Tokenizer* tokenizer;
    const TokenList* tokens;

    mu_check(tokenizer_new(input, &tokenizer) == ZENC_ERROR_OK);
    mu_check(tokenizer_tokenize(tokenizer) == ZENC_ERROR_OK);
    tokens = tokenizer_get_token_list(tokenizer);
    mu_check(tokens != NULL);

    TokenListIterator token_it;
    size_t i = 0;

    mu_check(token_list_iterator_init(&token_it, tokens) == ZENC_ERROR_OK);
    while (token_list_iterator_has_next(&token_it))
    {
        const Token* t = token_list_iterator_next(&token_it);
        mu_check(t != NULL);
        mu_check(i < sequence_length);
        mu_check(t->type == sequence[i].type);
        mu_check(strcmp(t->value, sequence[i].value) == 0);
        mu_check(t->line == sequence[i].line);
        mu_check(t->pos == sequence[i].pos);
        mu_check(t->length == sequence[i].length);
        i++;
    }
    mu_check(i == sequence_length);

    const TokenizerErrorList* error_list;
    error_list = tokenizer_get_errors(tokenizer);
    if (!errors || num_errors == 0)
    {
        mu_check(tokenizer_had_error(tokenizer) == false);
        mu_check(tokenizer_error_list_is_empty(error_list));
    }
    else
    {
        TokenizerErrorListIterator error_it;
        i = 0;

        mu_check(tokenizer_error_list_iterator_init(&error_it, error_list) == ZENC_ERROR_OK);
        while (tokenizer_error_list_iterator_has_next(&error_it))
        {
            const TokenizerError* err = tokenizer_error_list_iterator_next(&error_it);
            mu_check(err != NULL);
            mu_check(i < num_errors);
            mu_check(strcmp(err->token, errors[i].token) == 0);
            mu_check(err->line == errors[i].line);
            mu_check(err->pos == errors[i].pos);
            i++;
        }

        mu_check(i == num_errors);
    }

    tokenizer_free(tokenizer);
}

static Token make_token(TokenType type, const char* value, size_t line, size_t pos)
{
    Token t = 
    {
        .type = type,
        .value = (char*)value,
        .line = line,
        .pos = pos,
        .length = strlen(value)
    };
    return t;
}

MU_TEST(invalid_args) 
{
    Tokenizer* tokenizer;
    
    mu_check(tokenizer_new(NULL, &tokenizer) == ZENC_ERROR_INVALID_ARG);
    mu_check(tokenizer_new("", NULL) == ZENC_ERROR_INVALID_ARG);

    mu_check(tokenizer_tokenize(NULL) == ZENC_ERROR_INVALID_ARG);
    mu_check(tokenizer_get_token_list(NULL) == NULL);
    mu_check(tokenizer_had_error(NULL) == false);
    mu_check(tokenizer_get_errors(NULL) == NULL);
}

MU_TEST(keywords)
{
    expect_token_sequence("inhale", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "inhale", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("exhale", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "exhale", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("as", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "as", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("manifest", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "manifest", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("breathe", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "breath", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("into", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "into", 1, 1) }, 1, NULL, 0);
}

MU_TEST(identifiers)
{
    expect_token_sequence("myvar", (Token[]){ make_token(TOKEN_TYPE_IDENTIFIER, "myvar", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("inhale42", (Token[]){ make_token(TOKEN_TYPE_IDENTIFIER, "inhale42", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("inhaleexhaleas", (Token[]){ make_token(TOKEN_TYPE_IDENTIFIER, "inhaleexhaleas", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("_", (Token[]){ make_token(TOKEN_TYPE_IDENTIFIER, "_", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("_my_var_123", (Token[]){ make_token(TOKEN_TYPE_IDENTIFIER, "_my_var_123", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("_123", (Token[]){ make_token(TOKEN_TYPE_IDENTIFIER, "_123", 1, 1) }, 1, NULL, 0);
}

MU_TEST(numbers)
{
    expect_token_sequence("0", (Token[]){ make_token(TOKEN_TYPE_NUMBER, "0", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("1", (Token[]){ make_token(TOKEN_TYPE_NUMBER, "1", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("1234567890", (Token[]){ make_token(TOKEN_TYPE_NUMBER, "1234567890", 1, 1) }, 1, NULL, 0);
}

MU_TEST(strings)
{
    expect_token_sequence("\"\"", (Token[]){ make_token(TOKEN_TYPE_STRING, "\"\"", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\"Hello World!\"", (Token[]){ make_token(TOKEN_TYPE_STRING, "\"Hello World!\"", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\"1!2@3#4$5%%6^7&8*9(0)+}{:|<>?~\"", (Token[]){ make_token(TOKEN_TYPE_STRING, "\"1!2@3#4$5%%6^7&8*9(0)+}{:|<>?~\"", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\"😮💨\"", (Token[]){ make_token(TOKEN_TYPE_STRING, "\"😮💨\"", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\"\\\"\"", (Token[]){ make_token(TOKEN_TYPE_STRING, "\"\\\"\"", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\"\\\\\"", (Token[]){ make_token(TOKEN_TYPE_STRING, "\"\\\\\"", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\"\\\\\\\"\"", (Token[]){ make_token(TOKEN_TYPE_STRING, "\"\\\\\\\"\"", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\"Hello\\nWorld!\"", (Token[]){ make_token(TOKEN_TYPE_STRING, "\"Hello\\nWorld!\"", 1, 1) }, 1, NULL, 0);
}

MU_TEST(primitives)
{
    expect_token_sequence("+", (Token[]){ make_token(TOKEN_TYPE_PLUS, "+", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("-", (Token[]){ make_token(TOKEN_TYPE_MINUS, "-", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("*", (Token[]){ make_token(TOKEN_TYPE_ASTERISK, "*", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("/", (Token[]){ make_token(TOKEN_TYPE_SLASH, "/", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("%", (Token[]){ make_token(TOKEN_TYPE_PERCENT, "%", 1, 1) }, 1, NULL, 0);
}

MU_TEST(spaces)
{
    expect_token_sequence("", (Token[]){ }, 0, NULL, 0);
    expect_token_sequence("   ", (Token[]){ }, 0, NULL, 0);
    expect_token_sequence("     inhale       ", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "inhale", 1, 6) }, 1, NULL, 0);
    expect_token_sequence("\t\tinhale\t", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "inhale", 1, 3) }, 1, NULL, 0);
    expect_token_sequence("\n", (Token[]){ make_token(TOKEN_TYPE_LINEBREAK, "\n", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\r", (Token[]){ make_token(TOKEN_TYPE_LINEBREAK, "\r", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\r\n", (Token[]){ make_token(TOKEN_TYPE_LINEBREAK, "\r\n", 1, 1) }, 1, NULL, 0);
    expect_token_sequence("\n\n", (Token[]){ make_token(TOKEN_TYPE_LINEBREAK, "\n", 1, 1), make_token(TOKEN_TYPE_LINEBREAK, "\n", 2, 1) }, 2, NULL, 0);
    expect_token_sequence("\n\r", (Token[]){ make_token(TOKEN_TYPE_LINEBREAK, "\n", 1, 1), make_token(TOKEN_TYPE_LINEBREAK, "\r", 2, 1) }, 2, NULL, 0);
    expect_token_sequence("\n\r\n", (Token[]){ make_token(TOKEN_TYPE_LINEBREAK, "\n", 1, 1), make_token(TOKEN_TYPE_LINEBREAK, "\r\n", 2, 1) }, 2, NULL, 0);
}

MU_TEST(invalid_tokens)
{
    expect_token_sequence("!@#$^&()_{}:|<>?~", (Token[]){ make_token(TOKEN_TYPE_INVALID, "!@#$^&()_{}:|<>?~", 1, 1) }, 1, (TokenizerError[]) { { .token = "!@#$^&()_{}:|<>?~", .line = 1, .pos = 1 } }, 1);
    expect_token_sequence("🐢💨", (Token[]){ make_token(TOKEN_TYPE_INVALID, "🐢💨", 1, 1) }, 1, (TokenizerError[]) { { .token = "🐢💨", .line = 1, .pos = 1 } }, 1);
    expect_token_sequence("\"", (Token[]){ make_token(TOKEN_TYPE_INVALID, "\"", 1, 1) }, 1, (TokenizerError[]) { { .token = "\"", .line = 1, .pos = 1 } }, 1);
    expect_token_sequence("\"\\\"", (Token[]){ make_token(TOKEN_TYPE_INVALID, "\"\\\"", 1, 1) }, 1, (TokenizerError[]) { { .token = "\"\\\"", .line = 1, .pos = 1 } }, 1);
    expect_token_sequence("\"no end in sight", (Token[]){ make_token(TOKEN_TYPE_INVALID, "\"no end in sight", 1, 1) }, 1, (TokenizerError[]) { { .token = "\"no end in sight", .line = 1, .pos = 1 } }, 1);
    expect_token_sequence("\"\n", (Token[]){ make_token(TOKEN_TYPE_INVALID, "\"", 1, 1), make_token(TOKEN_TYPE_LINEBREAK, "\n", 1, 2) }, 2, (TokenizerError[]) { { .token = "\"", .line = 1, .pos = 1 } }, 1);
    expect_token_sequence("\"Hello \n linebreak\"", (Token[]){ make_token(TOKEN_TYPE_INVALID, "\"Hello ", 1, 1), make_token(TOKEN_TYPE_LINEBREAK, "\n", 1, 8), make_token(TOKEN_TYPE_IDENTIFIER, "linebreak", 2, 2), make_token(TOKEN_TYPE_INVALID, "\"", 2, 11) }, 4, (TokenizerError[]) { { .token = "\"Hello ", .line = 1, .pos = 1 }, { .token = "\"", .line = 2, .pos = 11 } }, 2);
    expect_token_sequence("01", (Token[]){ make_token(TOKEN_TYPE_INVALID, "01", 1, 1) }, 1, (TokenizerError[]) { { .token = "01", .line = 1, .pos = 1 } }, 1);
    expect_token_sequence("123abc", (Token[]){ make_token(TOKEN_TYPE_INVALID, "123abc", 1, 1) }, 1, (TokenizerError[]) { { .token = "123abc", .line = 1, .pos = 1 } }, 1);
    expect_token_sequence("42_", (Token[]){ make_token(TOKEN_TYPE_INVALID, "42_", 1, 1) }, 1, (TokenizerError[]) { { .token = "42_", .line = 1, .pos = 1 } }, 1);
}

MU_TEST(sequences)
{
    expect_token_sequence("inhale exhale", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "inhale", 1, 1), make_token(TOKEN_TYPE_KEYWORD, "exhale", 1, 8) }, 2, NULL, 0);
    expect_token_sequence("inhale\texhale", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "inhale", 1, 1), make_token(TOKEN_TYPE_KEYWORD, "exhale", 1, 8) }, 2, NULL, 0);
    expect_token_sequence("inhale\nexhale", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "inhale", 1, 1), make_token(TOKEN_TYPE_LINEBREAK, "\n", 1, 7), make_token(TOKEN_TYPE_KEYWORD, "exhale", 2, 1) }, 3, NULL, 0);
    expect_token_sequence("inhale\"str\"as\"str\"", (Token[]){ make_token(TOKEN_TYPE_KEYWORD, "inhale", 1, 1), make_token(TOKEN_TYPE_STRING, "\"str\"", 1, 7), make_token(TOKEN_TYPE_KEYWORD, "as", 1, 12), make_token(TOKEN_TYPE_STRING, "\"str\"", 1, 14) }, 4, NULL, 0);
    expect_token_sequence("bla\"bla\"", (Token[]){ make_token(TOKEN_TYPE_IDENTIFIER, "bla", 1, 1), make_token(TOKEN_TYPE_STRING, "\"bla\"", 1, 4) }, 2, NULL, 0);
    expect_token_sequence("+-*/%\"\"", (Token[]){ make_token(TOKEN_TYPE_PLUS, "+", 1, 1), make_token(TOKEN_TYPE_MINUS, "-", 1, 2), make_token(TOKEN_TYPE_ASTERISK, "*", 1, 3), make_token(TOKEN_TYPE_SLASH, "/", 1, 4), make_token(TOKEN_TYPE_PERCENT, "%", 1, 5), make_token(TOKEN_TYPE_STRING, "\"\"", 1, 6) }, 6, NULL, 0);
}

MU_TEST_SUITE(tokenizer_tests)
{
    MU_RUN_TEST(invalid_args);
    MU_RUN_TEST(keywords);
    MU_RUN_TEST(identifiers);
    MU_RUN_TEST(numbers);
    MU_RUN_TEST(strings);
    MU_RUN_TEST(primitives);
    MU_RUN_TEST(spaces);
    MU_RUN_TEST(invalid_tokens);
    MU_RUN_TEST(sequences);
}

int main()
{
    MU_RUN_SUITE(tokenizer_tests);
    MU_REPORT();
    return MU_EXIT_CODE;
}