#include "minunit/minunit.h"
#include "tokenizer/tokenizer.h"

MU_TEST(keywords) 
{
    Tokenizer* tokenizer;
    ZencError err = tokenizer_new(NULL, &tokenizer);
    mu_check(err == ZENC_ERROR_INVALID_ARG);
}

MU_TEST_SUITE(tokenizer_tests)
{
    MU_RUN_TEST(keywords);
}

int main()
{
    MU_RUN_SUITE(tokenizer_tests);
    MU_REPORT();
    return MU_EXIT_CODE;
}