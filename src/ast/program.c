#include "ast/program.h"

DEFINE_TYPED_LIST(Statement, statement, statement_free);
DEFINE_TYPED_ITERATOR(Statement, statement);