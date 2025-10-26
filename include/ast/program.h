#ifndef PROGRAM_H
#define PROGRAM_H

#include "ast/statements/statement.h"
#include "collections/typed_list.h"
#include "collections/typed_list_iterator.h"

DECLARE_TYPED_LIST(Statement, statement);
DECLARE_TYPED_ITERATOR(Statement, statement);

#endif