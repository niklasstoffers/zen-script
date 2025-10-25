#ifndef TOKENIZER_ERROR_LIST_H
#define TOKENIZER_ERROR_LIST_H

#include "tokenizer/tokenizer_error.h"
#include "collections/typed_list.h"
#include "collections/typed_list_iterator.h"

DECLARE_TYPED_LIST(TokenizerError, tokenizer_error);
DECLARE_TYPED_ITERATOR(TokenizerError, tokenizer_error);

#endif