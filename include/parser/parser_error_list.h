#ifndef PARSER_ERROR_LIST_H
#define PARSER_ERROR_LIST_H

#include "parser/parser_error.h"
#include "collections/typed_list.h"
#include "collections/typed_list_iterator.h"

DECLARE_TYPED_LIST(ParserError, parser_error);
DECLARE_TYPED_ITERATOR(ParserError, parser_error);

#endif