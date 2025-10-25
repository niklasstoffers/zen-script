#include "tokenizer/tokenizer_error_list.h"

DEFINE_TYPED_LIST(TokenizerError, tokenizer_error, tokenizer_error_free);
DEFINE_TYPED_ITERATOR(TokenizerError, tokenizer_error);