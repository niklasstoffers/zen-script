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
            i++;
        }

        mu_check(i == num_errors);
    }

    tokenizer_free(tokenizer);
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
    expect_token_sequence("inhale", (Token[]){ { .type = TOKEN_TYPE_KEYWORD, .value = "inhale" } }, 1, NULL, 0);
    expect_token_sequence("exhale", (Token[]){ { .type = TOKEN_TYPE_KEYWORD, .value = "exhale" } }, 1, NULL, 0);
    expect_token_sequence("as", (Token[]){ { .type = TOKEN_TYPE_KEYWORD, .value = "as" } }, 1, NULL, 0);
}

MU_TEST(identifiers)
{
    expect_token_sequence("myvar", (Token[]){ { .type = TOKEN_TYPE_IDENTIFIER, .value = "myvar" } }, 1, NULL, 0);
    expect_token_sequence("inhale42", (Token[]){ { .type = TOKEN_TYPE_IDENTIFIER, .value = "inhale42" } }, 1, NULL, 0);
    expect_token_sequence("inhaleexhaleas", (Token[]){ { .type = TOKEN_TYPE_IDENTIFIER, .value = "inhaleexhaleas" } }, 1, NULL, 0);
    expect_token_sequence("_", (Token[]){ { .type = TOKEN_TYPE_IDENTIFIER, .value = "_" } }, 1, NULL, 0);
    expect_token_sequence("_my_var_123", (Token[]){ { .type = TOKEN_TYPE_IDENTIFIER, .value = "_my_var_123" } }, 1, NULL, 0);
    expect_token_sequence("_123", (Token[]){ { .type = TOKEN_TYPE_IDENTIFIER, .value = "_123" } }, 1, NULL, 0);
}

MU_TEST(numbers)
{
    expect_token_sequence("0", (Token[]){ { .type = TOKEN_TYPE_NUMBER, .value = "0" } }, 1, NULL, 0);
    expect_token_sequence("1", (Token[]){ { .type = TOKEN_TYPE_NUMBER, .value = "1" } }, 1, NULL, 0);
    expect_token_sequence("1234567890", (Token[]){ { .type = TOKEN_TYPE_NUMBER, .value = "1234567890" } }, 1, NULL, 0);
}

MU_TEST(strings)
{
    expect_token_sequence("\"\"", (Token[]){ { .type = TOKEN_TYPE_STRING, .value = "\"\"" } }, 1, NULL, 0);
    expect_token_sequence("\"Hello World!\"", (Token[]){ { .type = TOKEN_TYPE_STRING, .value = "\"Hello World!\"" } }, 1, NULL, 0);
    expect_token_sequence("\"1!2@3#4$5%%6^7&8*9(0)+}{:|<>?~\"", (Token[]){ { .type = TOKEN_TYPE_STRING, .value = "\"1!2@3#4$5%%6^7&8*9(0)+}{:|<>?~\"" } }, 1, NULL, 0);
    expect_token_sequence("\"😮💨\"", (Token[]){ { .type = TOKEN_TYPE_STRING, .value = "\"😮💨\"" } }, 1, NULL, 0);
    expect_token_sequence("\"\\\"\"", (Token[]){ { .type = TOKEN_TYPE_STRING, .value = "\"\\\"\"" } }, 1, NULL, 0);
    expect_token_sequence("\"\\\\\"", (Token[]){ { .type = TOKEN_TYPE_STRING, .value = "\"\\\\\"" } }, 1, NULL, 0);
    expect_token_sequence("\"\\\\\\\"\"", (Token[]){ { .type = TOKEN_TYPE_STRING, .value = "\"\\\\\\\"\"" } }, 1, NULL, 0);
    expect_token_sequence("\"Hello\\nWorld!\"", (Token[]){ { .type = TOKEN_TYPE_STRING, .value = "\"Hello\\nWorld!\"" } }, 1, NULL, 0);
}

MU_TEST(spaces)
{
    expect_token_sequence("", (Token[]){ }, 0, NULL, 0);
    expect_token_sequence("   ", (Token[]){ }, 0, NULL, 0);
    expect_token_sequence("     inhale       ", (Token[]){ { .type = TOKEN_TYPE_KEYWORD, .value = "inhale" } }, 1, NULL, 0);
    expect_token_sequence("\t\tinhale\t", (Token[]){ { .type = TOKEN_TYPE_KEYWORD, .value = "inhale" } }, 1, NULL, 0);
    expect_token_sequence("\n", (Token[]){ { .type = TOKEN_TYPE_LINEBREAK, .value = "\n" } }, 1, NULL, 0);
    expect_token_sequence("\r", (Token[]){ { .type = TOKEN_TYPE_LINEBREAK, .value = "\r" } }, 1, NULL, 0);
    expect_token_sequence("\r\n", (Token[]){ { .type = TOKEN_TYPE_LINEBREAK, .value = "\r\n" } }, 1, NULL, 0);
    expect_token_sequence("\n\n", (Token[]){ { .type = TOKEN_TYPE_LINEBREAK, .value = "\n" }, { .type = TOKEN_TYPE_LINEBREAK, .value = "\n" } }, 2, NULL, 0);
    expect_token_sequence("\n\r", (Token[]){ { .type = TOKEN_TYPE_LINEBREAK, .value = "\n" }, { .type = TOKEN_TYPE_LINEBREAK, .value = "\r" } }, 2, NULL, 0);
    expect_token_sequence("\n\r\n", (Token[]){ { .type = TOKEN_TYPE_LINEBREAK, .value = "\n" }, { .type = TOKEN_TYPE_LINEBREAK, .value = "\r\n" } }, 2, NULL, 0);
}

MU_TEST(invalid_tokens)
{
    expect_token_sequence("!@#$%%^&*()_+{}:|<>?~", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "!@#$%%^&*()_+{}:|<>?~" } }, 1, (TokenizerError[]) { { .token = "!@#$%%^&*()_+{}:|<>?~" } }, 1);
    expect_token_sequence("🐢💨", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "🐢💨" } }, 1, (TokenizerError[]) { { .token = "🐢💨" } }, 1);
    expect_token_sequence("\"", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "\"" } }, 1, (TokenizerError[]) { { .token = "\"" } }, 1);
    expect_token_sequence("\"\\\"", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "\"\\\"" } }, 1, (TokenizerError[]) { { .token = "\"\\\"" } }, 1);
    expect_token_sequence("\"no end in sight", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "\"no end in sight" } }, 1, (TokenizerError[]) { { .token = "\"no end in sight" } }, 1);
    expect_token_sequence("\"\n", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "\"" }, { .type = TOKEN_TYPE_LINEBREAK, .value = "\n" } }, 2, (TokenizerError[]) { { .token = "\"" } }, 1);
    expect_token_sequence("\"Hello \n linebreak\"", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "\"Hello " }, { .type = TOKEN_TYPE_LINEBREAK, .value = "\n" }, { .type = TOKEN_TYPE_IDENTIFIER, .value = "linebreak" }, { .type = TOKEN_TYPE_INVALID, .value = "\"" } }, 4, (TokenizerError[]) { { .token = "\"Hello " }, { .token = "\"" } }, 2);
    expect_token_sequence("01", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "01" } }, 1, (TokenizerError[]) { { .token = "01" } }, 1);
    expect_token_sequence("123abc", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "123abc" } }, 1, (TokenizerError[]) { { .token = "123abc" } }, 1);
    expect_token_sequence("42_", (Token[]){ { .type = TOKEN_TYPE_INVALID, .value = "42_" } }, 1, (TokenizerError[]) { { .token = "42_" } }, 1);
}

MU_TEST_SUITE(tokenizer_tests)
{
    MU_RUN_TEST(invalid_args);
    MU_RUN_TEST(keywords);
    MU_RUN_TEST(identifiers);
    MU_RUN_TEST(numbers);
    MU_RUN_TEST(strings);
    MU_RUN_TEST(spaces);
    MU_RUN_TEST(invalid_tokens);
}

int main()
{
    MU_RUN_SUITE(tokenizer_tests);
    MU_REPORT();
    return MU_EXIT_CODE;
}