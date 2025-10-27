#include "parser/parser_error_list.h"

DEFINE_TYPED_LIST(ParserError, parser_error, parser_error_free);
DEFINE_TYPED_ITERATOR(ParserError, parser_error);