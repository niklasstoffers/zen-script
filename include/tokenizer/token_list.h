#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include "tokenizer/tokens.h"
#include "collections/typed_list.h"
#include "collections/typed_list_iterator.h"

DECLARE_TYPED_LIST(Token, token);
DECLARE_TYPED_ITERATOR(Token, token);

#endif