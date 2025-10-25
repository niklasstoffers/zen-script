#include "tokenizer/token_list.h"
#include "helpers/assertions.h"

DEFINE_TYPED_LIST(Token, token, token_free);
DEFINE_TYPED_ITERATOR(Token, token);